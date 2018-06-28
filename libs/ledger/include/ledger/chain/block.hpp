#ifndef CHAIN_BLOCK_HPP
#define CHAIN_BLOCK_HPP
#include "core/byte_array/referenced_byte_array.hpp"
#include "ledger/chain/transaction.hpp"
#include "core/serializers/byte_array_buffer.hpp"

#include <memory>

namespace fetch
{
namespace chain
{

struct BlockBody {
  fetch::byte_array::ByteArray previous_hash;
  fetch::byte_array::ByteArray merkle_hash;
  uint64_t block_number;
  std::vector<TransactionSummary> transactions; // TODO: (`HUT`) : slice these
                                                // TODO: (`HUT`) : proof
};

template <typename T>
void Serialize(T &serializer, BlockBody const &body)
{
  serializer << body.previous_hash << body.merkle_hash << body.transactions;
}

template <typename T>
void Deserialize(T &serializer, BlockBody &body)
{
  serializer >> body.previous_hash >> body.merkle_hash >> body.transactions;
}

template <typename P, typename H>
class BasicBlock
{
 public:
  typedef BlockBody                        body_type;
  typedef H                                hasher_type;

  body_type const &SetBody(body_type const &body) {
    body_ = body;
    return body_;
  }

  // Meta: Update hash
  void UpdateDigest() {

    serializers::ByteArrayBuffer buf;
    buf << body_.previous_hash << body_.merkle_hash << body_.block_number;
    hasher_type hash;
    hash.Reset();
    hash.Update(buf.data());
    hash.Final();
    hash_ = hash.digest();
  }

  body_type const &body() const { return body_; }
  fetch::byte_array::ByteArray const &hash() const { return hash_; }


  uint64_t &weight() { return weight_; }
  uint64_t &totalWeight() { return total_weight_; }
  bool &loose() { return is_loose_; }
  fetch::byte_array::ByteArray &root() { return root_; }

 private:
  body_type                    body_;
  fetch::byte_array::ByteArray hash_;

  // META data to help with block management
  uint64_t weight_        = 1; // TODO: (`HUT`) : think about weighting
  uint64_t total_weight_  = 1;
  bool is_loose_        = false;
  fetch::byte_array::ByteArray root_;

  template <typename AT, typename AP, typename AH>
  friend inline void Serialize(AT &serializer, BasicBlock<AP, AH> const &);

  template <typename AT, typename AP, typename AH>
  friend inline void Deserialize(AT &serializer, BasicBlock<AP, AH> &b);
};

template <typename T, typename P, typename H>
inline void Serialize(T &serializer, BasicBlock<P, H> const &b) {
  serializer << b.body_;
}

template <typename T, typename P, typename H>
inline void Deserialize(T &serializer, BasicBlock<P, H> &b) {
  BlockBody body;
  serializer >> body;
  b.SetBody(body);
}
}
}
#endif
