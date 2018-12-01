#include <efi.h>
#include <efilib.h>

#define KERNEL_PATH L"kernel\\kernel.bin"

#ifdef TARGET_QEMU
    // virt machine
    // memorymap: https://github.com/qemu/qemu/blob/master/hw/arm/virt.c
    // Ram start(0x40000000) + 1MiB
    #define KERNEL_ENTRYADDR (0x40000000 + 0x00100000)
#endif

typedef struct {
    EFI_MEMORY_DESCRIPTOR *memorymap;
    UINTN memorymap_size;
    UINTN mapkey;
    UINT32 description_version;
    UINTN description_size;
} efi_memory_map;


VOID
efi_panic(EFI_STATUS efi_status, INTN line)
{
    Print(L"panic at line:%d\n", line);
    Print(L"EFI_STATUS = %d\n", efi_status);
    while (1);
    // TODO: change while loop to BS->Exit
}

#define MY_EFI_ASSERT(status, line) if(EFI_ERROR(status)) efi_panic(status, line);

EFI_STATUS
efi_memory_attr2str(CHAR16 *attr_str, UINT64 attr)
{
    const UINT64 attr_table_val[12] = {
        EFI_MEMORY_UC,
        EFI_MEMORY_WC,
        EFI_MEMORY_WT,
        EFI_MEMORY_WB,
        EFI_MEMORY_UCE,
        EFI_MEMORY_WP,
        EFI_MEMORY_RP,
        EFI_MEMORY_XP,
        0, // EFI_MEMORY_NV,
        0, // EFI_MEMORY_MORE_RELIABLE,
        0, // EFI_MEMORY_RO,
        EFI_MEMORY_RUNTIME
    };
    const CHAR16 attr_table_str[12][32] = {
        L"UC, ",
        L"WC, ",
        L"WT, ",
        L"WB, ",
        L"UCE, ",
        L"WP, ",
        L"RP, ",
        L"XP, ",
        L"NV, ",
        L"MORE_RELIABLE, ",
        L"RO, ",
        L"RUNTIME, "
    };

    for (UINTN i = 0; i < 12; i++) {
        if (attr & attr_table_val[i]) {
            StrCat(attr_str, attr_table_str[i]);
        }
    }
    return EFI_SUCCESS;
}

EFI_STATUS
efi_memory_type2str(CHAR16 *str, UINTN type)
{
    // UEFI spec 2.5
    const CHAR16 type_table_str[15][32] = {
        L"ReservedMemoryType",
        L"LoaderCode",
        L"LoaderData",
        L"BootServicesCode",
        L"BootServicesData",
        L"RuntimeServicesCode",
        L"RuntimeServicesData",
        L"ConventionalMemory",
        L"UnusableMemory",
        L"ACPIReclaimMemory",
        L"ACPIMemoryNVS",
        L"MemoryMappedIO",
        L"MemoryMappedIOPortSpace",
        L"PalCode",
        L"MaxMemoryTyp"
    };

    StrCpy(str, type_table_str[type]);

    return EFI_SUCCESS;
}

EFI_STATUS
efi_show_memory_map(efi_memory_map *efi_memmap) 
{
    UINTN entry_count =  efi_memmap->memorymap_size / efi_memmap->description_size;
    Print(L"Show %d entry:\n", entry_count);

    Print(L"Idx : PA\tVA\tsize(KiB)\tType\tAttr\n");
    for (UINTN i = 0; i < entry_count; i++) {
        EFI_MEMORY_DESCRIPTOR *memmap = (EFI_MEMORY_DESCRIPTOR *)( 
            (UINTN)efi_memmap->memorymap + i * efi_memmap->description_size);
        
        EFI_PHYSICAL_ADDRESS pa = memmap->PhysicalStart;
        EFI_VIRTUAL_ADDRESS va = memmap->VirtualStart;
        UINTN size_mib = memmap->NumberOfPages * 4;

        CHAR16 attr_str[64] = L"";
        efi_memory_attr2str(attr_str, memmap->Attribute);
        CHAR16 type_str[64] = L"";
        efi_memory_type2str(type_str, memmap->Type);

        Print(L"%03d : %016lx\t%016lx\t%8d\t%s\t%s\n", i, pa, va, size_mib, type_str, attr_str);
    }

    return EFI_SUCCESS;
}



EFI_STATUS
efi_get_memory_map(efi_memory_map *efi_memmap) 
{
    EFI_STATUS efi_status = EFI_SUCCESS;
    // get memory map
    efi_memmap->memorymap_size = sizeof(EFI_MEMORY_DESCRIPTOR);
    efi_memmap->memorymap = NULL;

    efi_status = uefi_call_wrapper(BS->AllocatePool,
        3,
        EfiBootServicesData,
        efi_memmap->memorymap_size,
        (VOID **)&(efi_memmap->memorymap)
    );

    while (1) {
        efi_status = uefi_call_wrapper(
            BS->GetMemoryMap,
            5,
            &(efi_memmap->memorymap_size),
            efi_memmap->memorymap,
            &(efi_memmap->mapkey),
            &(efi_memmap->description_size),
            &(efi_memmap->description_version)
        );
        if (efi_status == EFI_BUFFER_TOO_SMALL) {
            // get more buffer
            efi_status = uefi_call_wrapper(
                BS->FreePool,
                1,
                (VOID *)(efi_memmap->memorymap)
            );
            efi_status = uefi_call_wrapper(
                BS->AllocatePool,
                3,
                EfiBootServicesData,
                efi_memmap->memorymap_size,
                (VOID **)&(efi_memmap->memorymap)
            );
        } else {
            // error or success
            MY_EFI_ASSERT(efi_status, __LINE__);
            // success
            break;
        }
    }
    return EFI_SUCCESS;
}


EFI_STATUS
efi_load_kernel(IN OUT EFI_PHYSICAL_ADDRESS *entry, OUT UINTN *pages, IN CHAR16 *path)
{
    EFI_STATUS efi_status;

    // get SIMPLE_FILE_SYSTEM_PROTOCOL
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL  *simple_fs_protocol;
    efi_status = uefi_call_wrapper(
        BS->LocateProtocol,
        3,
        &gEfiSimpleFileSystemProtocolGuid,
        NULL,
        (VOID **)&simple_fs_protocol
    );
    MY_EFI_ASSERT(efi_status, __LINE__);

    // Open Root volume
    EFI_FILE_PROTOCOL *root_fs_protocol;
    efi_status = uefi_call_wrapper(
        simple_fs_protocol->OpenVolume,
        2,
        simple_fs_protocol,
        &root_fs_protocol
    );
    MY_EFI_ASSERT(efi_status, __LINE__);

    // open kernel file
    EFI_FILE_PROTOCOL *kernel_fs_protocol;
    efi_status  = uefi_call_wrapper(
        root_fs_protocol->Open,
        5,
        root_fs_protocol,
        &kernel_fs_protocol,
        path,
        EFI_FILE_MODE_READ, 
        EFI_FILE_READ_ONLY
    );
    MY_EFI_ASSERT(efi_status, __LINE__);

    // read file info
    EFI_FILE_INFO finfo[2];
    UINTN finfo_size = sizeof(EFI_FILE_INFO) * 2;
    efi_status = uefi_call_wrapper(
        kernel_fs_protocol->GetInfo,
        4,
        kernel_fs_protocol,
        &gEfiFileInfoGuid,
        &finfo_size,
        finfo
    );
    MY_EFI_ASSERT(efi_status, __LINE__);

    // calc pagesize
    // pages = ceil(buffer_size / 4KiB)
    UINTN buffer_size = finfo->FileSize;
    *pages = (buffer_size + 4 * 1024 - 1) / (4 * 1024);

    Print(L"FileSize: %d\n", buffer_size);

    // allocate pages
    Print(L"Allocate: %16lx\n", *entry);
    efi_status = uefi_call_wrapper(
        BS->AllocatePages,
        4,
        AllocateAddress,
        EfiLoaderData,
        *pages,
        entry
    );
    MY_EFI_ASSERT(efi_status, __LINE__);
    Print(L"Allocated: %16lx\n", *entry);

    // load kernel to entry
    VOID *buffer = (VOID *)*entry;
    efi_status = uefi_call_wrapper(
        kernel_fs_protocol->Read,
        3,
        kernel_fs_protocol,
        &buffer_size,
        (VOID *)buffer
    );
    MY_EFI_ASSERT(efi_status, __LINE__);

    Print(L"Readed: %16lx\n", *entry);


    // close
    efi_status = uefi_call_wrapper(kernel_fs_protocol->Close, 1, kernel_fs_protocol);
    MY_EFI_ASSERT(efi_status, __LINE__);
    efi_status = uefi_call_wrapper(root_fs_protocol->Close, 1, root_fs_protocol);
    MY_EFI_ASSERT(efi_status, __LINE__);

    return EFI_SUCCESS;
}

EFI_STATUS
efi_main (EFI_HANDLE image_handle, EFI_SYSTEM_TABLE *systab)
{
    EFI_STATUS efi_status;
    efi_memory_map efi_memmap;

    InitializeLib(image_handle, systab);
    
    Print(L"Hello World!\n");

    // load kernel file to KERNEL_ENTRYADDR
    EFI_PHYSICAL_ADDRESS kernel_entry_addr = (EFI_PHYSICAL_ADDRESS)KERNEL_ENTRYADDR;
    UINTN pages = 0;
    CHAR16 *path = KERNEL_PATH;
    efi_status = efi_load_kernel(&kernel_entry_addr, &pages, path);
    MY_EFI_ASSERT(efi_status, __LINE__);

    Print(L"kernel loaded at %016lx - %016lx\n", 
        kernel_entry_addr, 
        kernel_entry_addr + pages * 4 * 1024
    );


    // Exit UEFI world!
    efi_status = efi_get_memory_map(&efi_memmap);
    MY_EFI_ASSERT(efi_status, __LINE__);

    // show memory map
    Print(L"Memorymap\n");
    efi_status = efi_show_memory_map(&efi_memmap);
    MY_EFI_ASSERT(efi_status, __LINE__);


    efi_status = uefi_call_wrapper(
        BS->ExitBootServices,
        2,
        image_handle, 
        efi_memmap.mapkey
    );
    MY_EFI_ASSERT(efi_status, __LINE__);

    // jump to init
    // KERNEL_ENTRYADDR : pass from cmake option
    int (*kernel_entry)() =  (int (*)())kernel_entry_addr;
    kernel_entry();
    //  not return

    efi_status = uefi_call_wrapper(BS->FreePages, 2, kernel_entry_addr, pages);
    MY_EFI_ASSERT(efi_status, __LINE__);

	return EFI_SUCCESS;
}
