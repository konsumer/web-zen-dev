#include <stdio.h>
#include "wasm_c_api.h"
#include "wasm_export.h"
#include "bh_read_file.h"


int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <WASM_FILE>\n", argv[0]);
    return 1;
  }

  static char global_heap_buf[512 * 1024];
  char *buffer, error_buf[128];
  const char *wasm_path = NULL, *wasi_dir = NULL;
  int opt, main_result = 1;

  wasm_module_t module = NULL;
  wasm_module_inst_t module_inst = NULL;
  wasm_exec_env_t exec_env = NULL;
  uint32 buf_size, stack_size = 8092, heap_size = 8092;

  RuntimeInitArgs init_args;
  memset(&init_args, 0, sizeof(RuntimeInitArgs));
  init_args.mem_alloc_type = Alloc_With_Pool;
  init_args.mem_alloc_option.pool.heap_buf = global_heap_buf;
  init_args.mem_alloc_option.pool.heap_size = sizeof(global_heap_buf);

 if (!wasm_runtime_full_init(&init_args)) {
      printf("Init runtime environment failed.\n");
      return -1;
  }

  buffer = bh_read_file_to_buffer(argv[1], &buf_size);

  if (!buffer) {
    printf("Open wasm app file [%s] failed.\n", wasm_path);
    goto fail;
  }

  module = wasm_runtime_load(buffer, buf_size, error_buf, sizeof(error_buf));
  if (!module) {
    printf("Load wasm module failed. error: %s\n", error_buf);
    goto fail;
  }

  // here is how to add dirs to WASI
  // https://github.com/bytecodealliance/wasm-micro-runtime/blob/e352f0ab101116c46a5a615d5139b70e8e9a3d47/core/iwasm/include/wasm_export.h#L715
  // wasm_runtime_set_wasi_args_ex(module, &wasi_dir, 1, NULL, 0, NULL, 0, NULL, 0, 0, 1, 2);
  
  module_inst = wasm_runtime_instantiate(module, stack_size, heap_size, error_buf, sizeof(error_buf));

  if (!module_inst) {
    printf("Instantiate wasm module failed. error: %s\n", error_buf);
    goto fail;
  }

  exec_env = wasm_runtime_create_exec_env(module_inst, stack_size);
  if (!exec_env) {
    printf("Create wasm execution environment failed.\n");
    goto fail;
  }

  if (wasm_application_execute_main(module_inst, 0, NULL)) {
    main_result = wasm_runtime_get_wasi_exit_code(module_inst);
  } else {
    printf("Call wasm function main failed. error: %s\n", wasm_runtime_get_exception(module_inst));
    goto fail;
  }

fail:
    if (exec_env) {
      wasm_runtime_destroy_exec_env(exec_env);
    }
    if (module_inst) {
      wasm_runtime_deinstantiate(module_inst);
    }
    if (module) {
      wasm_runtime_unload(module);
    }
    if (buffer) {
      BH_FREE(buffer);
    }
    wasm_runtime_destroy();
    return main_result;
}