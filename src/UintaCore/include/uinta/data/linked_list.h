#ifndef UINTA_LINKED_LIST_H
#define UINTA_LINKED_LIST_H

namespace uinta {

	class LinkedList {
		struct Node {
			uint32_t data;
			Node *next;

			explicit Node(uint32_t pData, Node *pNext = nullptr) : data(pData), next(pNext) {}

		}; // class Node

		Node *head;

	public:
		void append(uint32_t data) {
			if (head == nullptr) {
				head = new Node(data); // TODO pool these?
				return;
			}
			Node *current = head;
			while (current->next != nullptr) {
				current = current->next;
			}
			current->next = new Node(data);
		}

		void prepend(uint32_t data) {
			if (head == nullptr) {
				head = new Node(data);
				return;
			}
			Node *newHead = new Node(data);
			newHead->next = head;
			head = newHead;
		}

		void deleteWithValue(uint32_t data) {
			if (head == nullptr) return;
			if (head->data == data) {
				Node *pastHead = head;
				head = head->next;
				delete pastHead;
				return;
			}
			Node *current = head;
			while (current->next != nullptr) {
				if (current->next->data == data) {
					Node *pastNext = current->next;
					current->next = current->next->next;
					delete pastNext;
					return;
				}
				current = current->next;
			}
		}
	}; // class LinkedList

} // namespace uinta

#endif //UINTA_LINKED_LIST_H
