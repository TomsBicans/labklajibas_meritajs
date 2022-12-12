template<typename T>
class LinkedList {
  public:
    struct Node {
      T data;
      Node *next;
    };
  private:
    Node *head;
    Node *tail;
    int size;

  public:
    LinkedList() {
      head = nullptr;
      tail = nullptr;
      size = 0;
    }

    ~LinkedList() {
      clear();
    }

    int getSize() {
      return size;
    }

    bool isEmpty() {
      return size == 0;
    }

    void add(T data){
      if (size == 0){
        return addFirst(data);
      }else {
        return addLast(data);
      }
    }

    void addFirst(T data) {
      Node *node = new Node();
      node->data = data;
      node->next = head;
      head = node;
      if (tail == nullptr) {
        tail = head;
      }
      size++;
    }    

    void addLast(T data) {
      Node *node = new Node();
      node->data = data;
      node->next = nullptr;
      if (tail != nullptr) {
        tail->next = node;
        tail = node;
      } else {
        head = node;
        tail = node;
      }
      size++;
    }

    void clear() {
      while (head != nullptr) {
        Node *node = head;
        head = head->next;
        delete node;
        size--;
      }
      size = 0;
      tail = nullptr;
    }

    T getFirst() {
      if (isEmpty()) {
        return 0;
      }
      return head->data;
    }

    T getLast() {
      if (isEmpty()) {
        return 0;
      }
      return tail->data;
    }
    
    Node* getHead() {
    return head;
    }

    T removeFirst() {
      if (isEmpty()) {
        return 0;
      }
      Node *node = head;
      head = head->next;
      if (head == nullptr) {
        tail = nullptr;
      }
      T data = node->data;
      delete node;
      size--;
      return data;
    }

    T removeLast() {
      if (isEmpty()) {
        return 0;
      }
      if (head == tail) {
        return removeFirst();
      }
      Node *node = head;
      while (node->next != tail) {
        node = node->next;
      }
      tail = node;
      node = node->next;
      T data = node->data;
      delete node;
      size--;
      return data;
    }
};
