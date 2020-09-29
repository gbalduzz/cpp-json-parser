#include "gtest/gtest.h"

#include "json/json_reader.hpp"
#include "json/json_writer.hpp"

#include <complex>

TEST(ReadTest, All) {
  double d = 0.321;
  int i = 12;
  bool bt = true;
  bool bf = false;
  std::string s = "mela";
  std::vector<std::string> vs{"one", "two", "three"};
  std::vector<std::complex<double>> vc{std::complex<double>(0, 0), std::complex<double>(1, -1)};
  std::vector<std::vector<std::string>> vvs{std::vector<std::string>{"AA", "AB"},
                                            std::vector<std::string>{"BA", "BB"}};
  std::vector<std::vector<int>> vvi{std::vector<int>{0, 1, 2}, std::vector<int>{},
                                    std::vector<int>{3, 4}};

  auto read_or_write = [&](auto& file) {
    file.open_group("group 1");
    file.open_group("group 11");
    file.execute("scalar", d);
    file.execute("int", i);
    file.close_group();

    file.execute("string", s);

    file.open_group("group 12");
    file.execute("vec string", vs);
    file.close_group();

    file.open_group("group 2");
    file.close_group();

    file.open_group("group 3");
    file.execute("bool 1", bt);
    file.execute("bool 2", bf);
    file.close_group();

    file.execute("vec vec string", vvs);
    file.execute("vec vec int", vvi);
    file.execute("vec cmplx", vc);
  };

  json::JSONWriter writer;
  writer.open_file("writer_test.json");

  read_or_write(writer);
  writer.close_file();

  double d_check;
  std::swap(d, d_check);
  int i_check;
  std::swap(i, i_check);
  bool bt_check;
  std::swap(bt, bt_check);
  bool bf_check;
  std::swap(bf, bf_check);
  std::string s_check;
  std::swap(s, s_check);
  std::vector<std::string> vs_check;
  std::swap(vs, vs_check);
  std::vector<std::complex<double>> vc_check;
  std::swap(vc, vc_check);
  std::vector<std::vector<std::string>> vvs_check;
  std::swap(vvs, vvs_check);
  std::vector<std::vector<int>> vvi_check;
  std::swap(vvi, vvi_check);

  json::JSONReader reader;
  reader.open_file("writer_test.json");
  read_or_write(reader);

  EXPECT_EQ(d, d_check);
  EXPECT_EQ(i, i_check);
  EXPECT_EQ(bt, bt_check);
  EXPECT_EQ(bf, bf_check);
  EXPECT_EQ(s, s_check);
  EXPECT_EQ(vs, vs_check);
  EXPECT_EQ(vc, vc_check);
  EXPECT_EQ(vvs, vvs_check);
  EXPECT_EQ(vvi, vvi_check);
}
