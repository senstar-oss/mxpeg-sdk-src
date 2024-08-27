//           ///          //                                  C++ Cross Platform
//          /////        ////
//         /// XXX     XXX ///            ///////////   /////////     ///   ///
//        ///    XXX XXX    ///         ///             ///    ///   ///  ///
//       ///       XXX       ///         /////////     ///      //  //////
//      ///      XXX XXX      ///               ///   ///    ///   ///  ///
//     ////    XXX     XXX    ////    ////////////  //////////    ///   ///
//    ////                     ////
//   ////  M  O  B  O  T  I  X  ////////////////////////////////////////////////
//  //// Security Vision Systems //////////////////////////////////////////////
//
//  $Author: khe_admin $
//  $LastChangedBy: khe_admin $
//  $LastChangedDate: 2007-06-29 12:31:37 +0200 (Fri, 29 Jun 2007) $
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/MxPEGCoreComponent/trunk/include/mxcpcHuffmanTree.h $
//
///////
     //
     //  MOBOTIX MxPEG SDK
     //
     //  This file belongs to the C++ library released as part of the MxPEG SDK.
     //
     //  This software is licensed under the BSD licence,
     //  http://www.opensource.org/licenses/bsd-license.php
     //
     //  Copyright (c) 2005 - 2007, MOBOTIX AG
     //  All rights reserved.
     //
     //  Redistribution and use in source and binary forms, with or without
     //  modification, are permitted provided that the following conditions are
     //  met:
     //
     //  - Redistributions of source code must retain the above copyright
     //    notice, this list of conditions and the following disclaimer.
     //
     //  - Redistributions in binary form must reproduce the above copyright
     //    notice, this list of conditions and the following disclaimer in the
     //    documentation and/or other materials provided with the distribution.
     //
     //  - Neither the name of MOBOTIX AG nor the names of its contributors may
     //    be used to endorse or promote products derived from this software
     //    without specific prior written permission.
     //
     //  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
     //  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
     //  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
     //  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
     //  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
     //  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
     //  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
     //  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
     //  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
     //  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
     //  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
     //
///////



#ifndef __MXCPC_HUFFMANTREE_H__
#define __MXCPC_HUFFMANTREE_H__



//! Huffman tree with <tt>unsigned char</tt> valued leaves - used for 
//! <tt>MxPEG</tt> decoding.
/*!
 *  \ingroup mxcpc_core
 */
class mxcpcHuffmanTree {

 public:
  struct Node {
    Node *Child0, *Child1;
    bool IsLeaf;
    unsigned char Value;
  };
  
 private:
  Node *RootNode; 
  Node *GroundNode;

 public:
  mxcpcHuffmanTree();
  ~mxcpcHuffmanTree();
  
 public:
  //! Returns the Huffman tree's root node.
  const Node *getRoot(void);
   
  //! Clears the Huffman tree.
  void clear(void);
  //! Reconfigures the Huffman tree as specified in the given table 
  //! (JPEG Huffman table format).
  /*!
   *  Returns <tt>false</tt> on success, and <tt>true</tt> otherwise.
   *  In case of failue, the tree is cleared.
   */
  bool configureFromTable(const unsigned char *table, int max_path_len = 16);
  //! Adds a leaf that is reached from the root via a path of length 
  //! <tt>path_len</tt> bits.
  /*! 
   *  Returns <tt>false</tt> on success, and <tt>true</tt> otherwise.
   *  In any case the tree remains in defined state.
   */
  bool addLeaf(int path_len, unsigned char value);
  //! Returns the number of leaves in the tree.
  int countLeaves(void);
  
 private:
  //! Wastes the specified subtree.
  void deleteSubTree(Node *subtree_root);
  //! Adds to the specified subtree a leaf that is reached from the subtree 
  //! root via a path of length <tt>path_len</tt> bits.
  /*! 
   *  Returns <tt>false</tt> on success, and <tt>true</tt> otherwise.
   *  In any case the tree remains in defined state.
   */
  bool addLeaf(Node *subtree_root, int path_len, Node *leaf);
  //! Recursively makes unused child pointers of all inner nodes point to the
  //! special <i>ground node</i>, from which tree traversals never recover.
  /*!
   *  So if invalid bitstreams are encountered the tree traversal will forever
   *  be caught in the ground node and the Huffman decoder using the tree  will
   *  simply cease to produce video tiles - without the need of timeconsuming
   *  code to detect such invalid bitstreams.  
   */
  void groundUnusedPaths(Node *subtree);
  //! Private helper method.
  int countLeaves(Node *subtree);
};



#endif   // __MXCPC_HUFFMANTREE_H__
