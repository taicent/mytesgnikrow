#pragma once

/*++
Routine Description:

    Called to do formatted ascii dump to console of USB
    configuration, interface, and endpoint descriptors.

Arguments:

    Handle of Device

Return Value:

    String of human-readable string descriptor

--*/
CString GetUsbConfigDescriptor(HANDLE hDev);