/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include <fstream>

#include "gtest/gtest.h"

#include "logger/GlobalLogger.hpp"

int main(int argc, char** argv) {
    std::ofstream out("output.log");
    blocks::CreateGlobalLogger(out);
    ::testing::InitGoogleTest(&argc, argv);
    int retVal = RUN_ALL_TESTS();
    blocks::DestroyGlobalLogger();
    return retVal;
}
