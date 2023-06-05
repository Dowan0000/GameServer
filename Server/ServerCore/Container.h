#pragma once

#include "Types.h"
#include "Allocator.h"

#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

using namespace std;

template<typename T>
using Vector = vector<T, STLAllocator<T>>;

template<typename T>
using Lisg = list<T, STLAllocator<T>>;

template<typename Key, typename T, typename Pred = less<Key>>
using Map = map<Key, T, Pred, STLAllocator<pair<const Key, T>>>;

template<typename Key, typename Pred = less<Key>>
using Set = set<Key, Pred, STLAllocator<Key>>;

template<typename T>
using Deque = deque<T, STLAllocator<T>>;

template<typename T, typename Container = Deque<T>>
using Queue = queue<T, Container>;

template<typename T, typename Container = Deque<T>>
using Stack = stack<T, Container>;

template<typename T, typename Container = Vector<T>, typename Pred = less<typename Container::value_type>>
using PriorityQueue = priority_queue<T, Container, Pred>;

using String = basic_string<char, char_traits<char>, STLAllocator<char>>;

using WString = basic_string<wchar_t, char_traits<wchar_t>, STLAllocator<wchar_t>>;

template<typename Key, typename T, typename Hasher = hash<Key>, typename KeyEq = equal_to<Key>>
using HashMap = unordered_map<Key, T, Hasher, KeyEq, STLAllocator<pair<const Key, T>>>;

template<typename Key, typename Hasher = hash<Key>, typename KeyEq = equal_to<Key>>
using HashSet = unordered_set<Key, KeyEq, STLAllocator<Key>>;
