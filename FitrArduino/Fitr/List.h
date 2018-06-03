#ifndef LIST_H
#define LIST_H

template<typename T>
class Element {
    public:
        Element<T> *next;
        T value;

		Element(T t) : next(nullptr), value(t) {}
		Element(Element<T> *n, T t) : next(n), value(t) {}
		Element(Element<T> &other) : next(other.next), value(other.value) {}
};

template<typename T>
class List {
    private:
        Element<T> *first = nullptr;
        int _size = 0;
    public:
        List() {}

		List(const List<T> &other) {
			clear();
			addAll(other);
		}

		List(List<T> &&other) {
			clear();
			addAll(other);
		}

        template<typename ...Ts>
        List(Ts ...ts) {
            add(ts...);
        }

        ~List() {
            clear();
        }

		List<T>& operator=(List<T> &other) {
			clear();
			addAll(other);

			return *this;
		}

        void addAll(List<T> &&other) {
            for(int i = 0; i < other.size(); i++) {
                add(other[i]);
            }
        }

        void addAll(List<T> &other) {
            for(int i = 0; i < other.size(); i++) {
                add(other[i]);
            }
        }

		List<T>& operator+=(T t) {
			add(t);

			return *this;
		}

		List<T>& operator+=(List<T> other) {
			addAll(other);

			return *this;
		}

        void add(T t) {
            if(first == nullptr) {
                first = new Element<T>(t);
            } else {
                Element<T> *next = first;

                while(next->next != nullptr) {
                    next = next->next;
                }

                next->next = new Element<T>(t);
            }

            _size++;
        }

        template<typename ...Ts>
        void add(T t, Ts ...ts) {
            add(t);
            add(ts...);
        }

        bool addAt(int index, T t) {
            if(index == 0) {
                first = new Element<T>(first, t);
                _size++;

                return true;
            } else if(index < _size) {
                Element<T> *prior = nullptr;
                Element<T> *next = first;

                for(int i = 0; i < index; i++) {
                    prior = next;
                    next = next->next;
                }

                prior->next = new Element<T>(next, t);
                _size++;

                return true;
            }

            return false;
        }

        bool remove(int index) {
            if(index == 0) {
                if(first != nullptr) {
                    Element<T> *old = first;
                    first = old->next;

                    delete old;
                    _size--;
                }

                return true;
            } else if(index < _size) {
                Element<T> *prior = nullptr;
                Element<T> *next = first;

                for(int i = 0; i < index; i++) {
                    prior = next;
                    next = next->next;
                }

                prior->next = next->next;
                delete next;
                _size--;

                return true;
            }

            return false;
        }
		/*
		T& get(int index) const {
			return (*this)[index];
		}

		void set(int index, T t) {
			(*this)[index] = t;
		}*/

        T& operator[](int index) {
            Element<T> *next = first;

            for(int i = 0; i < index; i++) {
                next = next->next;
            }

            return next->value;
        }

        List<T> subset(int start, int end) {
            //if(start < 0) throw "Start index for list subset can't be less than zero";
            //if(end > _size) throw "End index for list subset greater than size";

            List<T> ss;

            for(int i = 0; i < 1 + end - start; i++) {
                ss.add((*this)[start + i]);
            }

            return ss;
        }

        int size() const {
            return _size;
        }

        T* array() {
            T *arr = new T[_size];

            for(int i = 0; i < _size; i++) {
				arr[i] = (*this)[i];
            }

            return arr;
        }

        void clear() {
            while(_size > 0) {
                remove(_size - 1);
            }
        }
};

#endif
