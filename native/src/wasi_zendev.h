#define ZENDEV_ROOT 3
#define ZENDEV_DEV 4
#define ZENDEV_FB 5
#define ZENDEV_DSP 6
#define ZENDEV_KEYBOARD 7
#define ZENDEV_MOUSE 8
#define ZENDEV_GAMEPAD 9

bool zendev_is_device(wasi_fd_t fd) {
  return fd == ZENDEV_FB || fd == ZENDEV_DSP || fd == ZENDEV_KEYBOARD || fd == ZENDEV_MOUSE || fd == ZENDEV_GAMEPAD;
}

// called by host, this will setup hardware
int zendev_setup(wasm_module_t module_inst) {
  return __WASI_ESUCCESS;
}

// called when a zendev device is opened
static wasi_errno_t zenfs_open(wasi_fd_t dirfd, wasi_fd_t* fd_app, char* path) {
  if (strncmp("fb", path, 2) == 0){
    *fd_app = ZENDEV_FB;
  }
  if (strncmp("dsp", path, 3) == 0){
    *fd_app = ZENDEV_DSP;
  }
  if (strncmp("event0", path, 6) == 0){
    *fd_app = ZENDEV_KEYBOARD;
  }
  if (strncmp("event1", path, 6) == 0){
    *fd_app = ZENDEV_MOUSE;
  }
  if (strncmp("event2", path, 6) == 0){
    *fd_app = ZENDEV_GAMEPAD;
  }

  printf("OPEN %d %s\n", *fd_app, path);
  return __WASI_ESUCCESS;
}

// called when a zendev device is written to by WASI
static wasi_errno_t zenfs_write(wasi_fd_t fd, unsigned char* buffer, uint64 size) {
  printf("WRITE %d: %zu\n", fd, size);
  return __WASI_ESUCCESS;
}

// called when a zendev device is closed by WASI
static wasi_errno_t zenfs_close(wasi_fd_t fd) {
  printf("CLOSE %d\n", fd);
  return __WASI_ESUCCESS;
}