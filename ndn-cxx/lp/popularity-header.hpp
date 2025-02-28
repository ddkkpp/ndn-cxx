#ifndef NDN_CXX_LP_POPULARITY_HEADER_HPP
#define NDN_CXX_LP_POPULARITY_HEADER_HPP

#include "ndn-cxx/encoding/block-helpers.hpp"
#include "ndn-cxx/encoding/encoding-buffer.hpp"

namespace ndn {
namespace lp {

class PopularityHeader
{
public:
  PopularityHeader() = default;

  explicit
  PopularityHeader(double value)
    : m_popularity(value)
  {
  }

  explicit
  PopularityHeader(const Block& block)
  {
    wireDecode(block);
  }

  double 
  getValue() const
  {
    return m_popularity;
  }

  template<encoding::Tag TAG>
  size_t
  wireEncode(EncodingImpl<TAG>& encoder) const
  {
    uint64_t value = static_cast<uint64_t>(m_popularity * 1000000.0);
    return encoder.prependNonNegativeInteger(value);
  }

  Block
  wireEncode() const
  {
    EncodingEstimator estimator;
    size_t totalLength = wireEncode(estimator);
    EncodingBuffer buffer(totalLength, 0);
    wireEncode(buffer);
    return buffer.block();
  }

  void
  wireDecode(const Block& wire)
  {
    m_popularity = static_cast<double>(readNonNegativeInteger(wire)) / 1000000.0;
  }

private:
  double m_popularity = 0.0;
};

} // namespace lp
} // namespace ndn

#endif // NDN_CXX_LP_POPULARITY_HEADER_HPP
