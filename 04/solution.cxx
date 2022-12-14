#include <algorithm>
#include <iostream>
#include <numeric>
#include <set>
#include <string>
#include <vector>

using Range = std::pair<int, int>;
using Pair = std::pair<Range, Range>;
using Pairs = std::vector<Pair>;

int parse_integer(std::string::const_iterator start, std::string::const_iterator finish)
{
  return std::stoi(std::string { start, finish });
}

Range parse_range(std::string::const_iterator start, std::string::const_iterator finish)
{
  auto hyphen = std::find(start, finish, '-');
  return { parse_integer(start, hyphen), parse_integer(std::next(hyphen), finish) };
}

Pair parse_line(std::string const& line)
{
  auto comma = std::find(line.cbegin(), line.cend(), ',');
  return { parse_range(line.cbegin(), comma), parse_range(std::next(comma), line.cend()) };
}

Pairs parse_input(std::istream& input)
{
  Pairs pairs;
  while (!input.eof()) {
    std::string line;
    std::getline(input, line);
    if (line.empty()) {
      continue;
    }
    pairs.push_back(parse_line(line));
  }
  return pairs;
}

bool contains(Range const& large, Range const small)
{
  return large.first <= small.first && large.second >= small.second;
}

bool larger(Range const& left, Range const& right)
{
  return left.first == right.first ? left.second >= right.second : left.first <= right.first;
}

bool solve(Pair const& pair)
{
  return larger(pair.first, pair.second) ? contains(pair.first, pair.second)
                                         : contains(pair.second, pair.first);
}

int solve(Pairs const& pairs)
{
  return std::transform_reduce(
      pairs.cbegin(), pairs.cend(), 0, std::plus<>(), [](auto const& pair) { return solve(pair); });
}

bool overlap(Range const& left, Range const& right)
{
  return left.first <= right.first && left.second <= right.second && left.second >= right.first;
}

bool solve_any_overlap(Pair const& pair)
{
  return overlap(pair.first, pair.second) || overlap(pair.second, pair.first)
      || solve(pair);
}

int solve_any_overlap(Pairs const& pairs)
{
  return std::transform_reduce(
      pairs.cbegin(), pairs.cend(), 0, std::plus<>(), [](auto const& pair) { return solve_any_overlap(pair); });
}

int main()
{
  auto input = parse_input(std::cin);
  std::cout << solve(input) << "\n";
  std::cout << solve_any_overlap(input) << "\n";
  return 0;
}
