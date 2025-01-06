cc_library(
    name = "lib",
    srcs = glob(["*.cc"],
    exclude = ["*_test.cc"]),
    hdrs = glob(["*.h"]),
    visibility = ["//visibility:public"],
)

cc_test(
    name = "padding_test",
    srcs = ["padding_test.cc"],
    deps = [
        ":lib",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)