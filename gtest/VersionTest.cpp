/**
 * @file      VersionTest.cpp
 * @author    Simon Brummer <simon.brummer@posteo.de>
 * @copyright 2017 Simon Brummer. All rights reserved.
 */

/*
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#include <gtest/gtest.h>
#include "Version.hpp"

using host_monitor::Version;

class VersionTest : public ::testing::Test
{
public:
    char const * const expected_major = "1";
    char const * const expected_minor = "0";
    char const * const expected_rev   = "0";
    char const * const expected_full  = "1.0.0";

    void SetUp(void)
    {
    }

    void TearDown(void)
    {
    }
};

TEST_F(VersionTest, MajorNumber)
{
    ASSERT_EQ( std::string_view(expected_major)
             , std::string_view(Version::major)
             );
}

TEST_F(VersionTest, MinorNumber)
{
    ASSERT_EQ( std::string_view(expected_minor)
             , std::string_view(Version::minor)
             );
}

TEST_F(VersionTest, RevisionNumber)
{
    ASSERT_EQ( std::string_view(expected_rev)
             , std::string_view(Version::rev)
             );
}

TEST_F(VersionTest, VersionNumber)
{
    ASSERT_EQ( std::string_view(expected_full)
             , std::string_view(Version::full)
             );
}
