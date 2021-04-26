#ifndef MY_STORAGE
#define MY_STORAGE
#include <iterator>

template<typename T>
class MyStorage
{
	struct Node
	{
		Node(const T& el, Node* prev=0, Node* next=0):_elem(el) , _prev(prev),_next(next)
		{
		}
        T _elem;
		Node* _prev;
		Node* _next;
	};

	Node* _top;
	Node* _bottom;
	int _size;
public:
    class iterator
	{
		Node* _cur;

		iterator(Node* node)
		{
			_cur = node;
		}
	public:
		friend class MyStorage;

		iterator& operator++()
        {
            if(_cur)
                _cur = _cur->_next;
			return *this;
		}
		iterator operator++(int)
		{
			Node* save_cur = _cur;
            if(_cur)
                _cur = _cur->_next;
			return iterator(save_cur);
		}
		iterator& operator--()
		{
            if(_cur)
                _cur = _cur->_prev;
			return *this;
		}
		iterator operator--(int)
		{
			Node* save_cur = _cur;
            if(_cur)
                _cur = _cur->_prev;
			return iterator(save_cur);
		}

		T& operator*()
		{
			return _cur->_elem;
		}
		bool operator!=(const iterator& rhs)
		{
			return _cur != rhs._cur;
		}
		bool operator==(const iterator& rhs)
		{
			return _cur == rhs._cur;
		}
        bool operator==(int rhs)
        {
            return _cur == rhs;
        }
        bool operator!=(int rhs)
        {
            return _cur != rhs;
        }

        /*int operator-(const iterator& rhs)const
        {
            return
        }*/
	};
    class reverse_iterator
    {
        Node* _cur;

        reverse_iterator(Node* node)
        {
            _cur = node;
        }
    public:
        friend class MyStorage;

        reverse_iterator& operator++()
        {
            if(_cur)
                _cur = _cur->_prev;
            return *this;
        }
        reverse_iterator operator++(int)
        {
            Node* save_cur = _cur;
            if(_cur)
                _cur = _cur->_prev;
            return reverse_iterator(save_cur);
        }
        reverse_iterator& operator--()
        {
            if(_cur)
                _cur = _cur->_next;
            return *this;
        }
        reverse_iterator operator--(int)
        {
            Node* save_cur = _cur;
            if(_cur)
                _cur = _cur->_next;
            return reverse_iterator(save_cur);
        }

        T& operator*()
        {
            return _cur->_elem;
        }
        bool operator!=(const reverse_iterator& rhs)
        {
            return _cur != rhs._cur;
        }
        bool operator==(const reverse_iterator& rhs)
        {
            return _cur == rhs._cur;
        }
        bool operator==(int rhs)
        {
            return _cur == rhs;
        }
        bool operator!=(int rhs)
        {
            return _cur != rhs;
        }
    };
public:
	MyStorage();
	MyStorage(int size_);
	~MyStorage();

	void push_back(const T& el);
	void pop_back();

	void push_front(const T& el);
	void pop_front();

	iterator insert(iterator pos, const T& el);
	iterator erase(iterator pos);

    int  size()const;
    void clear();

    template<typename Compare>
    void sort(Compare cmp);

	iterator begin()
	{
		return iterator(_top);
	}
	iterator end()
	{
        if(size()==0)
            return iterator(_top);
		return iterator(_bottom->_next);
	}
    reverse_iterator rbegin()
    {
        return reverse_iterator(_bottom);
    }
    reverse_iterator rend()
    {
        if(size()==0)
            return reverse_iterator(_bottom);
        return reverse_iterator(_top->_prev);
    }

};


template<typename T>
MyStorage<T>::MyStorage()
{
	_size = 0;
}


template<typename T>
MyStorage<T>::MyStorage(int size_)
{
	_size = 0;
	for (int i = 0; i < size_; i++)
		this->push_back(T());
}

template<typename T>
void MyStorage<T>::clear()
{
    while (size() != 0)
        pop_back();
}

template<class T>
template<class Compare>
void MyStorage<T>::sort(Compare cmp)
{
    for(iterator i = begin(); i != end();i++)
    {
        iterator prev = i;
        iterator elem = (iterator(i._cur->_next) != end()? iterator(i._cur->_next) : i);
        for(; cmp(*elem,*prev); --elem, --prev)
        {
            std::swap(*elem,*prev);
            if(prev == begin())
                break;
        }
    }
}

template<typename T>
void MyStorage<T>::push_back(const T& el)
{
	Node* new_node = new Node(el);
	if (size() == 0)
		_top = _bottom = new_node;
	else {
		new_node->_prev = _bottom;
		_bottom->_next = new_node;
		_bottom = new_node;
	}
	++_size;
}

template<typename T>
void MyStorage<T>::pop_back()
{
	Node* node_to_delete = _bottom;
	_bottom = _bottom->_prev;
	delete node_to_delete;
	--_size;
}

template<typename T>
void MyStorage<T>::push_front(const T& el)
{
	Node* new_node = new Node(el);
	if (size() == 0)
		_top = _bottom = new_node;
	else {
		new_node->_next = _top;
		_top->_prev = new_node;
		_top = new_node;
	}
	++_size;
}

template<typename T>
void MyStorage<T>::pop_front()
{
	Node* node_to_delete = _top;
	_top = _top->_next;
	delete node_to_delete;
	--_size;
}

template<typename T>
typename MyStorage<T>::iterator MyStorage<T>::insert(iterator pos, const T & el)
{
	Node* new_node = new Node(el);
	if (pos == begin())
	{
		new_node->_next = pos._cur;
		pos._cur->_prev = new_node;
		_top = new_node;
	}
	else if (pos == end())
	{
		_bottom->_next = new_node;
		new_node->_prev = _bottom;
		_bottom = new_node;
	}
	else
	{
		new_node->_next = pos._cur;
		new_node->_prev = pos._cur->_prev;

		pos._cur->_prev->_next = new_node;
		pos._cur->_prev = new_node;
	}
	
	++_size;
	return iterator(new_node);
}

template<typename T>
typename MyStorage<T>::iterator MyStorage<T>::erase(iterator pos)
{
    if (pos._cur == _top && pos._cur == _bottom)//size == 1
    {
        _top = _bottom = 0;
    }
    else if (pos._cur == _top)
	{
		_top = pos._cur->_next;
        _top->_prev = 0;
	}
	else if (pos._cur == _bottom)
	{
		_bottom = pos._cur->_prev;
        _bottom->_next = 0;
	}
	else
	{
		pos._cur->_prev->_next = pos._cur->_next;
		pos._cur->_next->_prev = pos._cur->_prev;
	}
	
	Node* next_node = pos._cur->_next;
	delete pos._cur;
	--_size;
	return iterator(next_node);
}

template<typename T>
MyStorage<T>::~MyStorage()
{
	while (size() != 0)
		pop_back();
}

template<typename T>
int MyStorage<T>::size()const
{
	return _size;
}
#endif
