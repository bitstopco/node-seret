{
  "targets": [{
    "target_name": "seret",
    "sources": ["./src/camera.c", "./src/seret.cc"],
    "cflags": ["-m32", "-Wall", "-Wextra", "-pedantic", "-ftrapv"],
    "cflags_c": ["-std=c11", "-Wno-unused-parameter"],
    "cflags_cc": ["-std=c++11"],
    "libraries": ["/opt/libjpeg-turbo/lib32/libturbojpeg.a"]
  }]
}

