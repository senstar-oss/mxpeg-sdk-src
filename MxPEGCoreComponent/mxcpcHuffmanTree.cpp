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
//  $HeadURL: http://svn.mobotix.net/svn/mxsdk/src/MxPEGCoreComponent/trunk/mxcpcHuffmanTree.cpp $
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



#include "include/mxcpcHuffmanTree.h"



mxcpcHuffmanTree::mxcpcHuffmanTree() {

  RootNode   = 0;
  GroundNode = 0;

  RootNode = new Node;
  GroundNode = new Node;
  
  RootNode->Child0 = 0;
  RootNode->Child1 = 0;
  RootNode->IsLeaf = false;
  
  GroundNode->Child0 = GroundNode;
  GroundNode->Child1 = GroundNode;
  GroundNode->IsLeaf = false;
}


mxcpcHuffmanTree::~mxcpcHuffmanTree() {

  clear();
  
  delete RootNode;
  delete GroundNode;
}



const mxcpcHuffmanTree::Node *mxcpcHuffmanTree::getRoot(void) {

  return(RootNode);
}


void mxcpcHuffmanTree::clear(void) {

  if(RootNode->Child0) {
    deleteSubTree(RootNode->Child0);
    RootNode->Child0 = 0;
  }
  if(RootNode->Child1) {
    deleteSubTree(RootNode->Child1);
    RootNode->Child1 = 0;
  }
}


bool mxcpcHuffmanTree::configureFromTable(const unsigned char *table, 
                                          int max_path_len) {
  
  const unsigned char *val_ptr;
  int symbol_num;
  int i, j;
  
  clear();
  
  val_ptr = table + 16;
  for(i = 1; i < max_path_len; i++) {
    symbol_num = table[i];
    for(j = 0; j < symbol_num; j++) {
      if(addLeaf(i + 1, *val_ptr)) {   // failure!
        clear();
        return(true);
      }
      else
        val_ptr++;
    }
  }
  
  groundUnusedPaths(RootNode);
  
  return(false);
}


int mxcpcHuffmanTree::countLeaves(void) {

  return(countLeaves(RootNode));
}


bool mxcpcHuffmanTree::addLeaf(int path_len, unsigned char value) {

  Node *leaf;
  bool result;
  
  leaf = new Node;
  
  leaf->Child0 = 0;
  leaf->Child1 = 0;
  leaf->IsLeaf = true;
  leaf->Value  = value;
  
  result = addLeaf(RootNode, path_len, leaf);
  
  if(result) delete leaf;
 
  return(result); 
}


bool mxcpcHuffmanTree::addLeaf(Node *subtree_root, int path_len, Node *leaf) {

  bool result;
  Node *new_node;
  
  if(subtree_root->IsLeaf) return(true);
  
  if(path_len == 1) {   // try to add leaf...
  
    if(!subtree_root->Child0) {
      subtree_root->Child0 = leaf;
      return(false);
    }
    else if(!subtree_root->Child1) { 
      subtree_root->Child1 = leaf;
      return(false);
    }
    else
      return(true);
  }
  
  else {
    
    // --- try 0-direction... ---
    // ensure we have inner 0-child node...
    if(!subtree_root->Child0) {
      
      new_node = new Node;  
      
      new_node->Child0 = 0;
      new_node->Child1 = 0;
      new_node->IsLeaf = false;
      subtree_root->Child0 = new_node;
    }
    // try 0-child subtree...
    result = addLeaf(subtree_root->Child0, path_len - 1, leaf);
    if(!result) return(false);
    
    // --- try 1-direction... --- 
    // ensure we have inner 1-child node...
    if(!subtree_root->Child1) {
      
      new_node = new Node;  
      
      new_node->Child0 = 0;
      new_node->Child1 = 0;
      new_node->IsLeaf = false;
      subtree_root->Child1 = new_node;
    }
    // try 1-child subtree...
    result = addLeaf(subtree_root->Child1, path_len - 1, leaf);
    if(!result) return(false);
    
    return(true);
  }
}


/*!
 *   Guards against killing the ground node.
 *
 *   Must not be called on the root node.
 */
void mxcpcHuffmanTree::deleteSubTree(Node *subtree_root) {

  if(subtree_root == GroundNode) return;

  if(subtree_root->Child0) deleteSubTree(subtree_root->Child0);
  if(subtree_root->Child1) deleteSubTree(subtree_root->Child1);
  
  delete subtree_root;
}


void mxcpcHuffmanTree::groundUnusedPaths(Node *subtree) {

  if(subtree->Child0) 
    groundUnusedPaths(subtree->Child0);
  else
    subtree->Child0 = GroundNode;
    
  if(subtree->Child1)
    groundUnusedPaths(subtree->Child1);
  else
    subtree->Child1 = GroundNode;
}


int mxcpcHuffmanTree::countLeaves(Node *subtree) {

  int num;
  
  if(subtree == GroundNode) return(0);
  
  if(subtree->IsLeaf) 
    return(1);
  else {
    num = 0;
    if(subtree->Child0) num += countLeaves(subtree->Child0);
    if(subtree->Child1) num += countLeaves(subtree->Child1);
    return(num);
  }
}
