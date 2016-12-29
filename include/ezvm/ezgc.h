#pragma once

template < class V, class T> class ezGC {
 public:
  void add(V* v);
  void subscribe(T* t);
  void unsubscribe(T* t);
};

template < class V, class T> void ezGC<V,T>::add(V* v) {
}

template < class V, class T> void ezGC<V,T>::subscribe(T* t) {
} 

template < class V, class T> void ezGC<V,T>::unsubscribe(T* t) {
} 
