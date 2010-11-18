#ifndef _FIBHEAP_H
#define _FIBHEAP_H

//***************************************************************************
//! \mainpage Fibonacci Heap
//! \author John Boyer
//! \author Doxygen'ed by Minh-Tri Pham
//! \date 1996
//!
//! The Fibonacci heap implementation contained in FIBHEAP.H and FIBHEAP.CPP
//! is Copyright (c) 1996 by John Boyer.
//!
//! Once this Fibonacci heap implementation (the software) has been published
//! by Dr. Dobb's Journal, permission to use and distribute the software is
//! granted provided that this copyright notice remains in the source and
//! the author (John Boyer) is acknowledged in works that use this program.
//!
//! Every effort has been made to ensure that this implementation is free of
//! errors.  Nonetheless, the author (John Boyer) assumes no liability regarding
//! your use of this software.
//!
//! The author would also be very glad to hear from anyone who uses the
//! software or has any feedback about it.
//! Email: jboyer@gulf.csc.uvic.ca
//!
//! Online: http://algorithm.diy.myrice.com/resources/technical_artile/fibonacci_heap/fibonacci.htm
//***************************************************************************

#define OK      0
#define NOTOK   -1

class FibHeap;

//======================================================
//! Fibonacci Heap Node Class
//======================================================

class FibHeapNode
{
friend class FibHeap;

     FibHeapNode *Left, *Right, *Parent, *Child;
     short Degree, Mark, NegInfinityFlag;

protected:

     inline int  FHN_Cmp(FibHeapNode& RHS);
     inline void FHN_Assign(FibHeapNode& RHS);

public:

     inline  FibHeapNode();
     virtual ~FibHeapNode();

     virtual void operator =(FibHeapNode& RHS);
     virtual int  operator ==(FibHeapNode& RHS);
     virtual int  operator <(FibHeapNode& RHS);
};

//========================================================================
//! Fibonacci Heap Class
//========================================================================

class FibHeap
{
     FibHeapNode *MinRoot;
     long NumNodes, NumTrees, NumMarkedNodes;

	 //! \brief A flag to tell if the heap \b owns the heap nodes.
	 //! If so, it is allowed to delete unused heap nodes.
     int HeapOwnershipFlag;

public:

     //! Default constructor
     FibHeap();
	 //! Default destructor
     virtual ~FibHeap();

// The Standard Heap Operations

	 //! \brief Inserts a new node into the heap; the new node must
	 //! contain a key value.
     void Insert(FibHeapNode *NewNode);
	 //! \brief Creates a new heap by joining two heaps given as input.
     void Union(FibHeap *OtherHeap);

	 //! \brief Returns a reference to the node containing the minimum
	 //! key value or some indication of what the key value is.
     inline FibHeapNode *Minimum();
	 //! \brief Returns the node of minimum value after removing it
	 //! from the heap.
     FibHeapNode *ExtractMin();

	 //! \brief Assigns a new, smaller key value to a node; the node
	 //! may need to be repositioned in the heap so that it is
	 //! extracted when there are no nodes with lesser values
	 //! than the NEW key value. A pointer to the node must be
	 //! given because heaps don't support an efficient search
	 //! operation.
     int DecreaseKey(FibHeapNode *theNode, FibHeapNode& NewKey);
	 //! \brief Deletes any node from the heap. A pointer to the node
	 //! must be given.
     int Delete(FibHeapNode *theNode);

// Extra utility functions

	 //! Returns heap ownership
     int  GetHeapOwnership() { return HeapOwnershipFlag; };
	 //! Set heap ownership -- heap owns heap nodes
     void SetHeapOwnership() { HeapOwnershipFlag = 1; };
	 //! Clear heap ownership -- heap does not own heap nodes
     void ClearHeapOwnership() { HeapOwnershipFlag = 0; };

	 //! Returns the number of nodes in the heap
     long GetNumNodes() { return NumNodes; };
	 //! Returns the number of trees in the heap
     long GetNumTrees() { return NumTrees; };
	 //! Returns the number of marked nodes in the heap
     long GetNumMarkedNodes() { return NumMarkedNodes; };

private:

// Internal functions that help to implement the Standard Operations

     inline void _Exchange(FibHeapNode*&, FibHeapNode*&);
     void _Consolidate();
     void _Link(FibHeapNode *, FibHeapNode *);
     void _AddToRootList(FibHeapNode *);
     void _Cut(FibHeapNode *, FibHeapNode *);
     void _CascadingCut(FibHeapNode *);
};

#endif
