#include "gtest/gtest.h"

#include "json/json_reader.hpp"

#include <complex>

TEST(ReadTest, ValidInput) {
  json::JSONReader reader;
  reader.open_file(TEST_DIR "read_test_input.json");

  double d;
  int i;
  bool b;
  std::string s;
  std::vector<std::string> vs;
  std::vector<std::complex<double>> vc;
  std::vector<std::vector<std::string>> vvs;
  std::vector<std::vector<int>> vvi;

  reader.open_group("group 1");
  EXPECT_THROW(reader.open_group("not a group"), std::logic_error);

  reader.open_group("group 11");
  reader.execute("scalar", d);
  EXPECT_DOUBLE_EQ(d, 3.1456);
  reader.execute("int", i);
  EXPECT_EQ(i, 42);
  EXPECT_THROW(reader.execute("not a field", i), std::logic_error);
  EXPECT_THROW(reader.execute("int", s), std::logic_error);
  EXPECT_EQ(i, 42);
  reader.close_group();

  reader.execute("string", s);
  EXPECT_EQ(s, "ciao");

  reader.open_group("group 12");
  reader.execute("vec string", vs);
  const std::vector<std::string> vs_check{"an apple", "a pear"};
  EXPECT_EQ(vs, vs_check);
  reader.close_group();

  reader.close_group();

  reader.open_group("group 2");
  reader.close_group();

  reader.open_group("group 3");
  reader.execute("bool", b);
  EXPECT_EQ(b, false);
  reader.execute("bool 2", b);
  EXPECT_EQ(b, true);
  reader.execute("bool 3", b);
  EXPECT_EQ(b, true);
  reader.execute("bool 4", b);
  EXPECT_EQ(b, false);
  reader.close_group();

  reader.execute("vec vec string", vvs);
  const std::vector<std::vector<std::string>> vvs_check{std::vector<std::string>{"aa", "ab"},
                                                        std::vector<std::string>{"ba", "bc"}};
  EXPECT_EQ(vvs, vvs_check);
  reader.execute("vec vec int", vvi);
  const std::vector<std::vector<int>> vvi_check{std::vector<int>{11, 12}, std::vector<int>{21, 11}};
  EXPECT_EQ(vvi, vvi_check);
  reader.execute("vec cmplx", vc);
  const std::vector<std::complex<double>> vc_check{std::complex<double>(1, -1),
                                                   std::complex<double>(2, 3)};
  EXPECT_EQ(vc, vc_check);
  // Complex can also be read as a pair.
  std::vector<std::pair<double, double>> v_pair;
  std::vector<std::pair<double, double>> v_pair_check;
  for (auto& c : vc_check)
    v_pair_check.push_back({c.real(), c.imag()});
  reader.execute("vec cmplx", v_pair);
  EXPECT_EQ(v_pair, v_pair_check);
}

TEST(ReadTest, InvalidInput) {
  auto test_file = [](const std::string& name, int err_line) {
    json::JSONReader reader;
    try {
      reader.open_file(name);
      EXPECT_TRUE(false);
    }
    catch (const std::logic_error& err) {
      const std::string msg = err.what();
      const std::string check = "Line " + std::to_string(err_line) + ":";
      EXPECT_NE(std::string::npos, msg.find(check));
    }
  };

  test_file(TEST_DIR "invalid_input1.json", 8);
  test_file(TEST_DIR "invalid_input2.json", 11);
  test_file(TEST_DIR "invalid_input3.json", 6);
}
