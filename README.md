# Riru - Template

[Riru](https://github.com/RikkaApps/Riru) module .
## NEW log
//http server
adb forward tcp:27041 tcp:27041 
//frida server
adb forward tcp:27042 tcp:27042 

modify load fride gradle.so

/sbin/.magisk/modules/riru_gadget

 def sourceDir = file("libs") <<--copy frida gradle.so to module libs folder
        if (sourceDir.exists() && sourceDir.isDirectory()) {
            copy {
                from sourceDir
                into "$magiskDir/lib"
                exclude "**/*.txt"
            }
        }

install->
  extract "$ZIPFILE" "lib/armeabi-v7a/libfrida-gadget.config.so" "$MAGISK_CURRENT_RIRU_MODULE_PATH/system/lib" true
  extract "$ZIPFILE" "lib/armeabi-v7a/libfrida-gadget.so" "$MAGISK_CURRENT_RIRU_MODULE_PATH/system/lib" true

gadget.conf
{
  "logging_enabled": true,
  "log_level": "info",
  "frida_version": "15.2.2",
  "app_list": [
    "com.android.app"
  ]
}
app_list-> hook app list


{
  "interaction": {
    "type": "connect",
    "address": "192.168.1.100",
    "port": 27042,
    "certificate": "/path/to/cert.pem",
    "token": "my-auth-token",
    "acl": ["192.168.1.0/24"],
    "parameters": {
      "resume_on_connect": true,
      "app_identifier": "com.example.app",
      "debug_mode": false,
      "custom_param": "value"
    }
  },
  "teardown": "minimal",
  "runtime": "qjs",
  "code_signing": "optional"
}

{
  "interaction": {
    "type": "listen",
    "address": "0.0.0.0",
    "port": 27042,
    "certificate": "/path/to/cert.pem",
    "token": "my-auth-token",
    "on_load": "resume",
    "on_port_conflict": "fail",
    "origin": "https://example.com",
    "asset_root": "/path/to/assets"
  },
  "teardown": "minimal",
  "runtime": "qjs",
  "code_signing": "optional"
}

## Build

1. Rename `module.example.gradle` to `module.gradle`
2. Replace module info in `module.gradle` (all lines end with "replace with yours")
3. Write your codes
4. Run gradle task `:module:assembleRelease` task from Android Studio or command line, zip will be saved in `out`.

## File structure

A Riru module is a Magisk module, please read [Magisk module document](https://topjohnwu.github.io/Magisk/guides.html#magisk-modules) first.

### Riru v24+

If the folder `$MODPATH/riru` exists, the module is considered as a Riru module. All files in `$MODPATH/riru/lib(64)` will be loaded by Riru.

### Riru pre-v24

If the folder `/data/adb/riru/modules/<name>` exists, `/system/lib(64)/libriru_<name>.so` will be loaded by Riru. Module installer needs to create this folder.

## Choose the minimum supported version

If your module is new, always choose the latest version.

In other cases, our recommendation is, if a new version Riru of released, raise the minimum supported version to latest when the new Riru comes stable.

## API changes

### API 24 (Riru v24)

- The Riru API version is unified with Riru version, now the API version is 24
- The `/data/adb/riru/modules/<name>` folder is deprecated, modules only need to place library files in `$MODPATH/riru/lib(64)` (see `customize.sh` `post-fs-data.sh`)
- The `init` function is called only once (see `main.cpp`)
- It's recommended to place modules files in the Magisk module folder, zygote has permission read this folder directly (the path is passed through `init` function, see `main.cpp`)

### API 10 (Riru v23)

<details>
  <summary><b>Background of rirud:</b></summary>

  Riru v22.0 move config files to `/data/adb`, this makes patch SELinux rules a must. However Magisk's `sepolicy.rule` actually not work for maybe lots of devices. As the release of Riru v22.0, these people "suddenly" appears.

  `sepolicy.rule` support was added from Magisk v20.2, a long time ago, no one report to Magisk 😒.

  To workaround this "problem", "rirud" is introduced. It will be started by `post-fs-data.sh` and run a socket runs under `u:r:zygote:s0` context. All file operations can be done through this socket.
</details>


From Riru v23, "read file" and "read dir" function are added for "rirud". Modules can use this to read files that zygote itself has not permission to access. Note, for hide purpose, "rirud" socket is only available before system_server is started.

In order to give the module enough freedom (like how to allocate memory), there is no "API". The module needs to implement socket codes by itself.

<details>

  <summary><b>Pseudocode of read file:</b></summary>

```
socket(PF_UNIX, SOCK_STREAM)
setup_sockaddr("rirud")

write(ACTION_READ_FILE /* 4 */, sizeof(uint32))
write(path_size, sizeof(uint32))
write(path, path_size)

errno = read(sizeof(int32_t)) // errno of "open" in "rirud"
if (errno != 0) return

bytes_count = read(sizeof(int32_t))

if (bytes_count > 0) {
  // file has size
  // read total "bytes_count" bytes
} else if (bytes_count == 0) {
  // file has no size, read until 0
  // read until 0
}
```

</details>

<details>

  <summary><b>Pseudocode of read dir:</b></summary>

```
socket(PF_UNIX, SOCK_STREAM)
setup_sockaddr("rirud")

write(ACTION_READ_DIR /* 5 */, sizeof(uint32))
write(path_size, sizeof(uint32))
write(path, path_size)

errno = read(sizeof(int32_t)) // errno of "opendir" in "rirud"
if (errno != 0) return

while (true) {
  write(1 /* continue */, sizeof(uint8))

  reply = read(sizeof(int32))
  if (reply == -1) break // end
  if (reply != 0) continue  // reply is errno of "readdir" in "rirud"

  d_type = read(sizeof(uchar))
  d_name = read(256)
}
```

</details>

Example implementation: <https://github.com/RikkaApps/Riru-LocationReportEnabler/commit/89b2e396efcd928121ba3d254b96af1560cfaf4d>

### API 9 (Riru v22)

#### API

Functions like `nativeForkAnd...` do not need to be exported directly. The only function to export is `void *init(void *)`. See the comment of `init` and template's implementation for more.

This has these advantages:

* Module can support different Riru versions
* Riru itself will not relay on ".prop" file (unreliable) to get module information

#### Riru

Starting v22.0, Riru has switched to "native bridge" (`ro.dalvik.vm.native.bridge`) to inject zygote, this will lead Riru and modules be loaded later ([`LoadNativeBridge`](https://cs.android.com/android/platform/superproject/+/android-11.0.0_r1:art/libnativebridge/native_bridge.cc;l=227) vs `__attribute__((constructor))`).

For most modules, this should have no problem, but modules like Xposed frameworks may have to make changes.

> Magisk may provider Riru-like features in the far future, and of course, it will have more strict restrictions, module codes will not be run in zygote. Maybe Xposed framework modules should prepare for this?

Riru v22 also provides hide function to make the memory of the module to anonymous memory ([see the implementation](https://github.com/RikkaApps/Riru/blob/master/core/src/main/cpp/hide.cpp)). This is an opt-in behavior (`module->supportHide`) and Riru itself also has a global toggle (`/data/adb/riru/enable_hide`).

#### Module installer

`RIRU_PATH` has been changed to `/data/adb/riru` for hide purpose. If you have other files in `/data/misc/riru`, move them here (or anywhere else if you want).

Note `/data/adb/riru` have the same SELinux like other Magisk files (set by Riru in post-fs-data), `u:object_r:magisk_file:s0`. DO NOT reset the context to something else.