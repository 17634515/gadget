SKIPUNZIP=1
# Extract verify.sh
ui_print "- Extracting verify.sh"
unzip -o "$ZIPFILE" 'verify.sh' -d "$TMPDIR" >&2
if [ ! -f "$TMPDIR/verify.sh" ]; then
  ui_print    "*********************************************************"
  ui_print    "! Unable to extract verify.sh!"
  ui_print    "! This zip may be corrupted, please try downloading again"
  abort "*********************************************************"
fi
. $TMPDIR/verify.sh

# Extract riru.sh

# Variables provided by riru.sh:
#
# RIRU_API: API version of installed Riru, 0 if not installed
# RIRU_MIN_COMPATIBLE_API: minimal supported API version by installed Riru, 0 if not installed or version < v23.2
# RIRU_VERSION_CODE: version code of installed Riru, 0 if not installed or version < v23.2
# RIRU_VERSION_NAME: version name of installed Riru, "" if not installed or version < v23.2

extract "$ZIPFILE" 'riru.sh' "$MODPATH"
. $MODPATH/riru.sh

# Functions from riru.sh
check_riru_version
enforce_install_from_magisk_app

# Check architecture
if [ "$ARCH" != "arm" ] && [ "$ARCH" != "arm64" ] && [ "$ARCH" != "x86" ] && [ "$ARCH" != "x64" ]; then
  abort "! Unsupported platform: $ARCH"
else
  ui_print "- Device platform: $ARCH"
fi

# Extract libs
ui_print "- Extracting module files"

extract "$ZIPFILE" 'module.prop' "$MODPATH"
extract "$ZIPFILE" 'post-fs-data.sh' "$MODPATH"
extract "$ZIPFILE" 'uninstall.sh' "$MODPATH"
extract "$ZIPFILE" 'gadget.conf' "$MODPATH"
extract "$ZIPFILE" 'service.sh' "$MODPATH"

# Extract gadget
ui_print "- Extracting gadget files"
unzip -o "$ZIPFILE" 'gadget/*' -d "$MODPATH" >&2
# Riru v24+ load files from the "riru" folder in the Magisk module folder
# This "riru" folder is also used to determine if a Magisk module is a Riru module
# In addition, Riru v24+ does not require the "libriru_" prefix, you can change this if this module does not need to support Riru pre-v24

ui_print "-load $MODPATH"
ui_print "-load $MAGISK_CURRENT_RIRU_MODULE_PATH"

mkdir "$MODPATH/riru"
mkdir "$MODPATH/riru/lib"
mkdir "$MODPATH/riru/lib64"

if [ "$ARCH" = "arm" ] || [ "$ARCH" = "arm64" ]; then
  ui_print "- Extracting arm libraries"
  extract "$ZIPFILE" "lib/armeabi-v7a/libriru_$RIRU_MODULE_ID.so" "$MODPATH/riru/lib" true
  extract "$ZIPFILE" "lib/armeabi-v7a/libfrida-gadget.config.so" "$MAGISK_CURRENT_RIRU_MODULE_PATH/system/lib" true
  extract "$ZIPFILE" "lib/armeabi-v7a/libfrida-gadget.so" "$MAGISK_CURRENT_RIRU_MODULE_PATH/system/lib" true
  extract "$ZIPFILE" "lib/armeabi-v7a/config" "$MODPATH" true
  if [ "$IS64BIT" = true ]; then
    ui_print "- Extracting arm64 libraries"
    extract "$ZIPFILE" "lib/arm64-v8a/libriru_$RIRU_MODULE_ID.so" "$MODPATH/riru/lib64" true
    extract "$ZIPFILE" "lib/arm64-v8a/libfrida-gadget.config.so" "$MAGISK_CURRENT_RIRU_MODULE_PATH/system/lib64" true
    extract "$ZIPFILE" "lib/arm64-v8a/libfrida-gadget.so" "$MAGISK_CURRENT_RIRU_MODULE_PATH/system/lib64" true
    extract "$ZIPFILE" "lib/arm64-v8a/config" "$MODPATH" true
  fi
fi

if [ "$ARCH" = "x86" ] || [ "$ARCH" = "x64" ]; then
  ui_print "- Extracting x86 libraries"
  extract "$ZIPFILE" "lib/x86/libriru_$RIRU_MODULE_ID.so" "$MODPATH/riru/lib" true

  if [ "$IS64BIT" = true ]; then
    ui_print "- Extracting x64 libraries"
    extract "$ZIPFILE" "lib/x86_64/libriru_$RIRU_MODULE_ID.so" "$MODPATH/riru/lib64" true
  fi
fi

# Riru pre-v24 uses "/system", "/data/adb/riru/modules" is used as the module list
# If "/data/adb/riru/modules/example" exists, Riru will try to load "/system/lib(64)/libriru_example.so

# If your module does not need to support Riru pre-v24, you can raise the value of "moduleMinRiruApiVersion" in "module.gradle"
# and remove this part

if [ "$RIRU_API" -lt 11 ]; then
  ui_print "- Using old Riru"
  mv "$MODPATH/riru" "$MODPATH/system"
  mkdir -p "/data/adb/riru/modules/$RIRU_MODULE_ID"
fi

set_perm_recursive "$MODPATH" 0 0 0755 0644