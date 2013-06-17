/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2013, Regents of the University of California
 *                     Alexander Afanasyev
 *
 * BSD license, See the LICENSE file for more information
 *
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#ifndef PERSISTENT_POLICY_H_
#define PERSISTENT_POLICY_H_

#include <boost/intrusive/options.hpp>
#include <boost/intrusive/list.hpp>

namespace ndn {
namespace trie {

/**
 * @brief Traits for persistent replacement policy
 *
 * In this policy entries are added until there is a space (controlled by set_max_size call).
 * If maximum is reached, new entries will not be added and nothing will be removed from the container
 */
struct persistent_policy_traits
{
  /// @brief Name that can be used to identify the policy (e.g., for logging)
  static std::string GetName () { return "Persistent"; }

  struct policy_hook_type : public boost::intrusive::list_member_hook<> {};

  template<class Container>
  struct container_hook
  {
    typedef boost::intrusive::member_hook< Container,
                                           policy_hook_type,
                                           &Container::policy_hook_ > type;
  };

  template<class Base,
           class Container,
           class Hook>
  struct policy 
  {
    typedef typename boost::intrusive::list< Container, Hook > policy_container;
    
    // could be just typedef
    class type : public policy_container
    {
    public:
      typedef Container parent_trie;
    
      type (Base &base)
        : base_ (base)
        , max_size_ (100) // when 0, policy is not enforced
      {
      }

      inline void
      update (typename parent_trie::iterator item)
      {
        // do nothing
      }
  
      inline bool
      insert (typename parent_trie::iterator item)
      {
        if (max_size_ != 0 && policy_container::size () >= max_size_)
          return false;
      
        policy_container::push_back (*item);
        return true;
      }
  
      inline void
      lookup (typename parent_trie::iterator item)
      {
        // do nothing
      }
  
      inline void
      erase (typename parent_trie::iterator item)
      {
        policy_container::erase (policy_container::s_iterator_to (*item));
      }

      inline void
      clear ()
      {
        policy_container::clear ();
      }

      inline void
      set_max_size (size_t max_size)
      {
        max_size_ = max_size;
      }

      inline size_t
      get_max_size () const
      {
        return max_size_;
      }

    private:
      // type () : base_(*((Base*)0)) { };

    private:
      Base &base_;
      size_t max_size_;
    };
  };
};

} // trie
} // ndn

#endif // PERSISTENT_POLICY_H_
