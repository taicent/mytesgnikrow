#ifndef __SD_AVL_H
#define __SD_AVL_H




/*==========================================================================

	   _____                .__          .__          
	  /     \   ____   _____|  |__  __ __|  | _____   
	 /  \ /  \_/ __ \ /  ___/  |  \|  |  \  | \__  \  
	/    Y    \  ___/ \___ \|   Y  \  |  /  |__/ __ \_
	\____|__  /\___  >____  >___|  /____/|____(____  /
	        \/     \/     \/     \/                \/ 
	   _________   ____.____      ___________                      
	  /  _  \   \ /   /|    |     \__    ___/______   ____   ____  
	 /  /_\  \   Y   / |    |       |    |  \_  __ \_/ __ \_/ __ \ 
	/    |    \     /  |    |___    |    |   |  | \/\  ___/\  ___/ 
	\____|__  /\___/   |_______ \   |____|   |__|    \___  >\___  >
	        \/                 \/                        \/     \/ 

	with thanks to -
		Greig McLachlan of http://www.learntoprogram.com
			for providing a clean implementation of Insert which I used as a starting point
		Jason F Boxall of CSC330
			who posted his assignment on the web, I double checked my Remove implementation against his.
		R Levow of http://www.cse.fau.edu/~roy/cop3530.98f/prog5-BSTIterator.html
			who posted an algorithm for creating binary tree iterators

	http://meshula.artistnation.com
										2002

	The template is free for use, no warrantee or suitability for any particular 
	purpose is expressed or implied. Verify that it works the way you expect before you 
	incorporate it into your own project.

	public interface -
	
		AVL();
		~AVL();
		bool  Insert(KeyType key, ItemType item);
		bool  Remove(KeyType key);
		bool  Find(KeyType key, ItemType& item, CondType cond = EQUAL);
		short GetDepth()
		short GetSize()
		short Count(KeyType key, CondType cond = EQUAL); 
		
		Iterator(Iterator*)
		Iterator(AVL*)
		~Iterator()
		
			// all return true for success, false for failure or end of tree

			bool GetFirst(KeyType& key, ItemType& item)
			bool GetNext(KeyType& key, ItemType& item)
			bool GetCurrent(KeyType& key, ItemType& item)
			bool Find(KeyType key, ItemType& item, CondType cond = EQUAL)

  ==========================================================================*/

#ifndef ABSTRACT_CONDITION_TYPE_
#define ABSTRACT_CONDITION_TYPE_

enum CondType
  {
    EQUAL = 1,
    LESS = 2,
    GREATER = 4,
    LESS_EQUAL = EQUAL | LESS,
    GREATER_EQUAL = EQUAL | GREATER
  };

#endif



/*--------------------------------------------------------------------------------------------
	    ___     ___           _           _                 _   _             
	   / \ \   / / |       __| | ___  ___| | __ _ _ __ __ _| |_(_) ___  _ __  
	  / _ \ \ / /| |      / _` |/ _ \/ __| |/ _` | '__/ _` | __| |/ _ \| '_ \ 
	 / ___ \ V / | |___  | (_| |  __/ (__| | (_| | | | (_| | |_| | (_) | | | |
	/_/   \_\_/  |_____|  \__,_|\___|\___|_|\__,_|_|  \__,_|\__|_|\___/|_| |_|
  --------------------------------------------------------------------------------------------*/

template<class KeyType>
class AVL
{
protected:
	class AVLNode
	{
	public:
		AVLNode(KeyType key = 0) :
			m_Balance(0), m_Depth(1), m_Cnt(1), m_TotalCnt(1),
			m_Key(key), 
			m_pLeft(0), m_pRight(0)
		{
		}
		
		short		m_Balance;
		short		m_Depth;
		long		m_Cnt; // the number of keys of the same value
		long		m_TotalCnt; // the total number of keys of the branch
		
		KeyType		m_Key;

		AVLNode*	m_pLeft;
		AVLNode*	m_pRight;
	};

	AVLNode* m_pRoot;
	
	int m_maxNodes, m_nNodes;
	AVLNode *m_Nodes;

public:

	/*------------------------------------------------------------------------
		             _     _ _        _       _             __                
		 _ __  _   _| |__ | (_) ___  (_)_ __ | |_ ___ _ __ / _| __ _  ___ ___ 
		| '_ \| | | | '_ \| | |/ __| | | '_ \| __/ _ \ '__| |_ / _` |/ __/ _ \
		| |_) | |_| | |_) | | | (__  | | | | | ||  __/ |  |  _| (_| | (_|  __/
		| .__/ \__,_|_.__/|_|_|\___| |_|_| |_|\__\___|_|  |_|  \__,_|\___\___|
		|_|                                                                    */

	AVL(int maxNodes) : m_pRoot(0)
	{
		m_maxNodes = maxNodes;
		m_nNodes = 0;
		m_Nodes = new AVLNode [maxNodes];
	}
	
	AVL(int maxNodes, KeyType *array, long len) : m_pRoot(0)
	{
		m_maxNodes = maxNodes;
		m_nNodes = 0;
		m_Nodes = new AVLNode [maxNodes];
		while (--len >= 0) Insert(array[len]);
	}

	~AVL() { delete[] m_Nodes; }
	
	void 	Insert(KeyType key);
	bool 	Remove(KeyType key);
	bool 	Find  (KeyType key, CondType cond, 
					long &minIndex, long &maxIndex, KeyType &outKey);
	bool	GetKey(long index, long &minIndex, long &maxIndex, KeyType &outKey);
					
	short	GetDepth() const {	return (m_pRoot ? m_pRoot->m_Depth : 0);	}
	long	GetTotalCnt() const {	return (m_pRoot ? m_pRoot->m_TotalCnt : 0);	}
	
protected:
	AVLNode*	newNode(KeyType key)
	{
		AVLNode* p = &m_Nodes[m_nNodes++];
		p->m_Key = key;
		return p;
	}

protected:
	void _Insert        		(KeyType key, AVLNode*& root);
	void _Remove				(AVLNode*& root, KeyType key, bool& delOK);
	void _RemoveBothChildren	(AVLNode*& root, AVLNode*& curr, bool& delOK);
	bool _GetKey				(long index, long ofs, AVLNode* root,
									long &minIndex, long &maxIndex, KeyType &outKey);
	bool _GetNode				(long ofs, AVLNode* root,
									long &minIndex, long &maxIndex, KeyType &outKey);

	void ComputeBalance 		(AVLNode*  root);
	void Balance        		(AVLNode*& root);
	void BalanceRight			(AVLNode*& root);
	void BalanceLeft			(AVLNode*& root);
	void RotateLeft				(AVLNode*& root);
	void RotateRight			(AVLNode*& root);
};


/*--------------------------------------------------------------------------------------------
	 _                     _   _             
	(_)_ __  ___  ___ _ __| |_(_) ___  _ __  
	| | '_ \/ __|/ _ \ '__| __| |/ _ \| '_ \ 
	| | | | \__ \  __/ |  | |_| | (_) | | | |
	|_|_| |_|___/\___|_|   \__|_|\___/|_| |_|
  --------------------------------------------------------------------------------------------*/

template<class KeyType>
void AVL<KeyType>::Insert(KeyType key)
{
	if (m_pRoot == 0)
	{
		m_pRoot = newNode(key);
	}
	else
		_Insert(key, m_pRoot);
}

template<class KeyType>
void AVL<KeyType>::_Insert(KeyType key, AVLNode*& root)
{
	if (key < root->m_Key)
	{
		if (root->m_pLeft)
			_Insert(key, root->m_pLeft);
		else
			root->m_pLeft = newNode(key);
	}
	else if (key > root->m_Key)
	{
		if (root->m_pRight)
			_Insert(key, root->m_pRight);
		else
			root->m_pRight = newNode(key);
	}
	else ++root->m_Cnt;
	
	ComputeBalance(root);
	Balance(root);
}

/*--------------------------------------------------------------------------------------------
	                                    _ 
	 _ __ ___ _ __ ___   _____   ____ _| |
	| '__/ _ \ '_ ` _ \ / _ \ \ / / _` | |
	| | |  __/ | | | | | (_) \ V / (_| | |
	|_|  \___|_| |_| |_|\___/ \_/ \__,_|_|
  --------------------------------------------------------------------------------------------*/
                                      
template<class KeyType>
bool AVL<KeyType>::Remove(KeyType key)
{
	bool delOK = false;
	_Remove(m_pRoot, key, delOK);
	return delOK;
}

template<class KeyType>
void AVL<KeyType>::_Remove(AVLNode*& root, KeyType key, bool& delOK)
{
	if (!root)
	{
		delOK = false;
	}
	else if (root->m_Key > key)	// go to left subtree
	{
		_Remove(root->m_pLeft, key, delOK);

		if (delOK)
		{
			ComputeBalance(root);
			BalanceRight(root);
		}
	}
	else if (root->m_Key < key) // go to right subtree
	{
		_Remove(root->m_pRight, key, delOK);

		if (delOK)
		{
			ComputeBalance(root);
			BalanceLeft(root);
		}
	}
	else if (root->m_Cnt > 1) // node found! and got more than 1 key
	{
		--root->m_Cnt;
		delOK = true;
		ComputeBalance(root);
	}
	else	// node found! and only 1 key left
	{
		AVLNode* pMe = root;
		
		if (!root->m_pRight)
		{
			root = root->m_pLeft;
			delOK = true;
			delete pMe;
		}
		else if (!root->m_pLeft)
		{
			root = root->m_pRight;
			delOK = true;
			delete pMe;
		}
		else
		{
			_RemoveBothChildren(root, root->m_pLeft, delOK);
			if (delOK)
			{
				ComputeBalance(root);
				Balance(root);
			}
				
			delOK = true;
		}
	}
}

template<class KeyType>
void AVL<KeyType>::_RemoveBothChildren(AVLNode*& root, AVLNode*& curr, bool& delOK)
{
	if (!curr->m_pRight)
	{
		root->m_Key = curr->m_Key;
		root->m_Cnt = curr->m_Cnt;
		AVLNode* pMe = curr;
		curr = curr->m_pLeft;
		delete pMe;
		delOK = true;
	}
	else
	{
		_RemoveBothChildren(root, curr->m_pRight, delOK);
		if (delOK)
		{
			ComputeBalance(root);
			Balance(root);
		}
	}
}

/*--------------------------------------------------------------------------------------------
	                         _     _             
	 ___  ___  __ _ _ __ ___| |__ (_)_ __   __ _ 
	/ __|/ _ \/ _` | '__/ __| '_ \| | '_ \ / _` |
	\__ \  __/ (_| | | | (__| | | | | | | | (_| |
	|___/\___|\__,_|_|  \___|_| |_|_|_| |_|\__, |
	                                       |___/ 
  --------------------------------------------------------------------------------------------*/

template<class KeyType>
bool AVL<KeyType>::_GetNode(long ofs, AVLNode *root,
	long &minIndex, long &maxIndex, KeyType &outKey)
{
	if (root->m_pLeft) minIndex = ofs + root->m_pLeft->m_TotalCnt;
	else minIndex = ofs;
	maxIndex = minIndex + root->m_Cnt - 1;
	outKey = root->m_Key;
	return true;
}


template<class KeyType>
bool AVL<KeyType>::Find(KeyType key, CondType cond,
	long &minIndex, long &maxIndex, KeyType &outKey)
{
	long ofs, ofsMin, ofsMax;
	AVLNode *root = m_pRoot, *minNode = 0, *maxNode = 0;

	ofs = 0;
	while (root)
	{
		if ((key == root->m_Key) && (cond & EQUAL))
			return _GetNode(ofs, root, minIndex, maxIndex, outKey);
		if (key < root->m_Key ||
			((key == root->m_Key) && (cond & LESS)))
		{
			ofsMax = ofs;
			maxNode = root;
			root = root->m_pLeft;
		}
		else
		{
			ofsMin = ofs;
			minNode = root;
			ofs += (root->m_pLeft? root->m_pLeft->m_TotalCnt : 0) + root->m_Cnt;
			root = root->m_pRight;
		}
	}
	
	if (cond == EQUAL) return false;
	if (cond & LESS)
	{
		if (minNode) return _GetNode(ofsMin, minNode, minIndex, maxIndex, outKey);
	}
	else
	{
		if (maxNode) return _GetNode(ofsMax, maxNode, minIndex, maxIndex, outKey);
	}
	return false;
}


template<class KeyType>
bool AVL<KeyType>::GetKey(long index, 
	long &minIndex, long &maxIndex, KeyType &outKey)
{
	if(!m_pRoot || index < 0 || index >= m_pRoot->m_TotalCnt) return false;
	return _GetKey(index, 0, m_pRoot, minIndex, maxIndex, outKey);
}

template<class KeyType>
bool AVL<KeyType>::_GetKey(long index, long ofs, AVLNode* root,
	long &minIndex, long &maxIndex, KeyType &outKey)
{
	long cnt = root->m_pLeft? root->m_pLeft->m_TotalCnt: 0;
	if(index < cnt) return _GetKey(index, ofs, root->m_pLeft, minIndex, maxIndex, outKey );
	if(index < cnt+root->m_Cnt) return _GetNode(ofs, root, minIndex, maxIndex, outKey );
	cnt += root->m_Cnt;
	return _GetKey(index-cnt, ofs+cnt, root->m_pRight, minIndex, maxIndex, outKey );
}


/*--------------------------------------------------------------------------------------------
	 _           _                  _             
	| |__   __ _| | __ _ _ __   ___(_)_ __   __ _ 
	| '_ \ / _` | |/ _` | '_ \ / __| | '_ \ / _` |
	| |_) | (_| | | (_| | | | | (__| | | | | (_| |
	|_.__/ \__,_|_|\__,_|_| |_|\___|_|_| |_|\__, |
	                                        |___/ 
  --------------------------------------------------------------------------------------------*/

template<class KeyType>
void AVL<KeyType>::ComputeBalance(AVLNode* root)
{
	if (root)
	{
		short leftDepth  = root->m_pLeft  ? root->m_pLeft->m_Depth  : 0;
		short rightDepth = root->m_pRight ? root->m_pRight->m_Depth : 0;

		root->m_Depth = 1 + ((leftDepth > rightDepth) ? leftDepth : rightDepth);
		root->m_Balance = rightDepth - leftDepth;

		root->m_TotalCnt = root->m_Cnt;
		if (root->m_pLeft ) root->m_TotalCnt += root->m_pLeft->m_TotalCnt;
		if (root->m_pRight) root->m_TotalCnt += root->m_pRight->m_TotalCnt;
	}
}

template<class KeyType>
void AVL<KeyType>::Balance(AVLNode*& root)
{
	// AVL trees have the property that no branch is more than 1 longer than its sibling

	if (root->m_Balance > 1)
		BalanceRight(root);
		
	if (root->m_Balance < -1)
		BalanceLeft(root);
}

template<class KeyType>
void AVL<KeyType>::BalanceRight(AVLNode*& root)
{
	if (root->m_pRight)
	{
		if (root->m_pRight->m_Balance > 0)
		{
			RotateLeft(root);
		}
		else if (root->m_pRight->m_Balance < 0)
		{
			RotateRight(root->m_pRight);
			RotateLeft(root);
		}
	}
}

template<class KeyType>
void AVL<KeyType>::BalanceLeft(AVLNode*& root)
{
	if (root->m_pLeft)
	{
		if (root->m_pLeft->m_Balance < 0)
		{
			RotateRight(root);
		}
		else if (root->m_pLeft->m_Balance > 0)
		{
			RotateLeft(root->m_pLeft);
			RotateRight(root);
		}
	}
}

template<class KeyType>
void AVL<KeyType>::RotateLeft(AVLNode*& root)
{
	AVLNode* pTemp = root;
	root = root->m_pRight;
	pTemp->m_pRight = root->m_pLeft;
	root->m_pLeft = pTemp;

	ComputeBalance(root->m_pLeft);
	ComputeBalance(root->m_pRight);
	ComputeBalance(root);
}

template<class KeyType>
void AVL<KeyType>::RotateRight(AVLNode*& root)
{
	AVLNode* pTemp = root;
	root = root->m_pLeft;
	pTemp->m_pLeft = root->m_pRight;
	root->m_pRight = pTemp;

	ComputeBalance(root->m_pLeft);
	ComputeBalance(root->m_pRight);
	ComputeBalance(root);
}

#endif
