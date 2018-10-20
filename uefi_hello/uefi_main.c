#include <efi.h>


EFI_SYSTEM_TABLE *ST;
EFI_BOOT_SERVICES *BS;

EFI_STATUS
efi_puts(CHAR16 *s)
{
    ST->ConOut->OutputString(ST->ConOut, s);
    return EFI_SUCCESS;
}

EFI_STATUS
efi_main (EFI_HANDLE image_handle, EFI_SYSTEM_TABLE *systab)
{
    EFI_STATUS efi_status;

    ST = systab;
    BS = systab->BootServices;
    
    efi_status = efi_puts(L"Hello World!\n");

	return EFI_SUCCESS;
}
