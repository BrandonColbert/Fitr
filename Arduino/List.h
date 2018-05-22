#ifndef LIST_H
#define LIST_H

template<typename T>
class Element {
    public:
        Element<T> *next;
        T value;

        Element(Element<T> *next, T t) {
            this->next = next;
            value = t;
        }

        Element(T t) {
            next = nullptr;
            value = t;
        }
};

template<typename T>
class List {
    private:
        Element<T> *first;
        int _size;
    public:
        List() {}

        template<typename ...Ts>
        List(Ts ...ts) {
            add(ts...);
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

        T operator[](int index) {
            Element<T> *next = first;

            for(int i = 0; i < index; i++) {
                next = next->next;
            }

            return next->value;
        }

        int size() {
            return _size;
        }
};

#endif
