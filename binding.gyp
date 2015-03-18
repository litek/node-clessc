{
  "targets": [
    {
      "target_name": "addon",
      "sources": [
        "addon.cc",
        "<!@(ls -1 vendor/clessc/src/*.cpp)",
        "<!@(ls -1 vendor/clessc/src/value/*.cpp)"
      ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")",
        "vendor/clessc/src",
        "config"
      ],
      "cflags_cc!": [ "-fno-rtti", "-fno-exceptions" ],
      "cflags!": [ "-fno-exceptions" ],
      "conditions": [
        [ 'OS=="mac"', {
          "xcode_settings": {
            "OTHER_CPLUSPLUSFLAGS" : ["-std=c++11","-stdlib=libc++", "-v"],
            "OTHER_LDFLAGS": ["-stdlib=libc++"],
            "MACOSX_DEPLOYMENT_TARGET": "10.7",
            "GCC_ENABLE_CPP_EXCEPTIONS": "YES"
          }
        }]
      ]
    }
  ]
}
