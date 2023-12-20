#pragma once

#include "Util.h"



template<typename K, typename V>
class UnorderedHashMap
{
private:
	typedef int Ref;

	struct Node
	{
        Ref prev;
        Ref next;
		K key;
	};

	typedef const V* ConstIterator;

	const size_t tableSize;
	const size_t hashMask;
	Ref* const table;
	Ref* const tableBase;

	Node* const nodes;
	V*    const values;
	Node* tail;
	V*    valuesTail;
	Node* bound;
	Node* const nodesBase;
	V*    const valuesBase;

public:
	UnorderedHashMap(size_t capacity) noexcept
	:
		tableSize(next_power_of_two(capacity)),
		hashMask(tableSize - 1),
		table(array_calloc<Ref>(tableSize)),
		tableBase(table + tableSize),

		nodes (array_malloc<Node>(capacity)),
		values(array_malloc<V>(capacity)),
		tail(nodes),
		valuesTail(values),
		bound(nodes + capacity),
		nodesBase(nodes - 1), // To use 1-based indexing
		valuesBase(values - 1) // To use 1-based indexing
	{
		assert(table && nodes && values);
	}


	~UnorderedHashMap() noexcept
	{
		std::free(table);
		std::free(nodes);
		std::free(values);
	}


	void insert(K key, const V& value) noexcept
	{
		Ref* slot = table + position(key);
		Ref slotRef = Ref(slot - tableBase);

		Node* node = tail;
//        if (!(tail < bound))
//            printf("problem on r%d\n", key);
		assert(tail < bound);
		tail++;

		Ref nodeRef = Ref(node - nodesBase);

		node->prev = slotRef;
		node->next = *slot;
		node->key  = key;
		if (node->next)
			nodesBase[node->next].prev = nodeRef;
		*slot = nodeRef;

		*valuesTail = value;
		valuesTail++;
	}



	void erase(K key) noexcept
	{
		Ref* slot = table + position(key);
		Ref slotRef = Ref(slot - tableBase);

		Node* node = nodesBase + *slot;


		while (node->key != key) // Will contain the key, may skip checks for null
		{
			slotRef = *slot;
			slot = &node->next;
			node = nodesBase + *slot;
		}

		V* currentValue = valuesBase + *slot;

		*slot = node->next;
		if (node->next)
			nodesBase[node->next].prev = slotRef;

		tail--;
		valuesTail--;
		if (node != tail)
		{
			*currentValue = *valuesTail;
			node->prev  = tail->prev;
			node->next  = tail->next;
			node->key   = tail->key;
			Ref nodeRef = Ref(node - nodesBase);

			if (node->next)
				nodesBase[node->next].prev = nodeRef;

            Ref yo = node->prev;
			if (yo < 0)
				tableBase[yo] = nodeRef;
			else
				nodesBase[yo].next = nodeRef;
		}
	}


    V operator[] (K key) noexcept
    {
        Ref* slot = table + position(key);
        Ref slotRef = Ref(slot - tableBase);
        
        Node* node = nodesBase + *slot;
        
        
        while (node->key != key) // Will contain the key, may skip checks for null
        {
            slotRef = *slot;
            slot = &node->next;
            node = nodesBase + *slot;
        }
        
        return (*(valuesBase + *slot));
    }






	inline size_t position(K key) const noexcept
	{
		return key & hashMask;
	}


	ConstIterator begin() const noexcept
	{
		return values;
	}


	ConstIterator end() const noexcept
	{
		return valuesTail;
	}


	size_t size() const noexcept
	{
		return end() - begin();
	}



	UnorderedHashMap& operator=(const UnorderedHashMap&) = delete;
	UnorderedHashMap(const UnorderedHashMap&) = delete;

};

