
#include "stdafx.h"


#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <cassert>
#include <ctime>
#include <map>

template<class T>
class Tree{
private:
	struct Node{
		Node*parent, *left, *right;
		T value;

		explicit Node(const T&value) :
			parent(0), left(0), right(0), value(value){}
	};

	Node*root;
public:
	Tree() :root(0){}

	bool is_empty()const{
		return !root;
	}

	// ------------------------------------------------------------------ //

private:
	static bool is_left_child(const Node*node){
		if (node->parent == 0){
			return false;
		}
		else{
			return node->parent->left == node;
		}
	}

	static bool is_right_child(const Node*node){
		if (node->parent == 0){
			return false;
		}
		else{
			return node->parent->right == node;
		}
	}

	Node**get_parent_ptr(Node*node){
		if (node->parent == 0){
			return &root;
		}
		else if (is_left_child(node)){
			return &node->parent->left;
		}
		else{
			return &node->parent->right;
		}
	}

	static std::string format_label(const Node*node){
		if (node){
			std::ostringstream out;
			out << node->value;
			return out.str();
		}
		else{
			return "";
		}
	}

	static unsigned get_height(const Node*node){
		if (!node){
			return 0;
		}
		unsigned left_height = 0, right_height = 0;

		if (node->left){
			left_height = get_height(node->left);
		}
		if (node->right){
			right_height = get_height(node->right);
		}

		return std::max(left_height, right_height) + 1;
	}

	static unsigned get_width(const Node*node){
		if (!node){
			return 0;
		}
		unsigned left_width = 0, right_width = 0;

		if (node->left){
			left_width = get_width(node->left);
		}
		if (node->right){
			right_width = get_width(node->right);
		}

		return left_width + format_label(node).length() + right_width;
	}

	static void dump_spaces(std::ostream&out, unsigned count){
		for (unsigned i = 0; i<count; ++i)
			out.put(' ');
	}

	static void dump_line(std::ostream&out, const Node*node, unsigned line){
		if (!node){
			return;
		}
		if (line == 1){
			// the root will be shown here
			dump_spaces(out, get_width(node->left));
			out << format_label(node);
			dump_spaces(out, get_width(node->right));
		}
		else{
			dump_line(out, node->left, line - 1);
			dump_spaces(out, format_label(node).length());
			dump_line(out, node->right, line - 1);
		}
	}

	static void dump_node(std::ostream&out, const Node*node){
		for (unsigned line = 1, height = get_height(node); line <= height; ++line){
			dump_line(out, node, line);
			out.put('\n');
		}
		out.flush();
	}

public:
	void dump(std::ostream&out)const{
		dump_node(out, root);
	}

private:
	bool is_wellformed(Node*node){
		if (node == 0){
			return true;
		}
		if (*get_parent_ptr(node) != node){
			return false;
		}
		if (!is_wellformed(node->left)){
			return false;
		}
		if (!is_wellformed(node->right)){
			return false;
		}
		return true;
	}

	// ------------------------------------------------------------------ //

public:
	~Tree(){
		Node*now = root;
		while (now){
			if (now->left){
				now = now->left;
			}
			else if (now->right){
				now = now->right;
			}
			else{
				Node*next = now->parent;
				*get_parent_ptr(now) = 0;
				delete now;
				now = next;
			}
		}
	}

	Tree(const Tree&other){
		if (other.is_empty())
			root = 0;
		else{
			root = new Node(other.root->value);
			try{
				Node
					*now = root,
					*other_now = other.root;
				while (other_now){
					if (other_now->left && !now->left){
						now->left = new Node(other_now->left->value);
						now->left->parent = now;
						now = now->left;
						other_now = other_now->left;
					}
					else if (other_now->right && !now->right){
						now->right = new Node(other_now->right->value);
						now->right->parent = now;
						now = now->right;
						other_now = other_now->right;
					}
					else{
						now = now->parent;
						other_now = other_now->parent;
					}
					assert(is_wellformed(root));
				}
			}
			catch (...){
				this->~Tree();
				throw;
			}
		}
	}

public:
	void swap(Tree&other){
		std::swap(root, other.root);
	}

	Tree&operator=(const Tree&other){
		Tree temp(other);
		swap(temp);
		return *this;
	}

	// ------------------------------------------------------------------ //

private:
	template<class NodeT>
	static NodeT*search(NodeT*root, const T&value){
		NodeT*now = root;
		while (now){
			if (value < now->value){
				now = now->left;
			}
			else if (now->value < value){
				now = now->right;
			}
			else{
				return now;
			}
		}
		return 0;
	}

public:
	bool contains(const T&t)const{
		return search(root, t);
	}

	// ------------------------------------------------------------------ //

public:
	bool add(const T&value){
		Node
			*parent = 0,
			*now = root;
		bool is_left_child = false;
		while (now){
			parent = now;
			if (value < now->value){
				is_left_child = true;
				now = now->left;
			}
			else if (now->value < value){
				is_left_child = false;
				now = now->right;
			}
			else{
				return false; // the element is already in the tree
			}
		}
		Node*new_node = new Node(value);
		new_node->parent = parent;

		if (parent == 0){
			root = new_node;
		}
		else if (is_left_child){
			parent->left = new_node;
		}
		else{
			parent->right = new_node;
		}

		assert(is_wellformed(root));
		return true;
	}

	// ------------------------------------------------------------------ //

private:
	void swap_near_nodes(Node*child, Node*parent){
		*get_parent_ptr(parent) = child;

	
		// swap child and parent pointer
		std::swap(parent->left, child->left);
		std::swap(parent->right, child->right);
		std::swap(parent->parent, child->parent);

		

		if (child->left == child){
			child->left = parent;
		}
		else{
			child->right = parent;
		}

		//parent pointer will be adjusted here
		if (child->left){
			child->left->parent = child;
		}
		if (child->right){
			child->right->parent = child;
		}
		if (parent->left){
			parent->left->parent = parent;
		}
		if (parent->right){
			parent->right->parent = parent;
		}

		assert(is_wellformed(root));
	}

	void swap_far_nodes(Node*a, Node*b){
		// update parent pointer
		*get_parent_ptr(a) = b;
		*get_parent_ptr(b) = a;

		// update child pointer
		if (a->left){
			a->left->parent = b;
		}
		if (a->right){
			a->right->parent = b;
		}
		if (b->left){
			b->left->parent = a;
		}
		if (b->right){
			b->right->parent = a;
		}

	
		std::swap(a->left, b->left);
		std::swap(a->right, b->right);
		std::swap(a->parent, b->parent);

		assert(is_wellformed(root));
	}

	void swap_nodes(Node*a, Node*b){
		if (a->parent == b){
			swap_near_nodes(a, b);
		}
		else if (b->parent == a){
			swap_near_nodes(b, a);
		}
		else{
			swap_far_nodes(a, b);
		}
	}

	// ------------------------------------------------------------------ //

private:
	template<class NodeT>
	static NodeT*get_min(NodeT*node){
		NodeT*now = node;
		while (now->left)
			now = now->left;
		return now;
	}

	template<class NodeT>
	static NodeT*get_max(NodeT*node){
		NodeT*now = node;
		while (now->right)
			now = now->right;
		return now;
	}
private:
	template<class NodeT>
	static NodeT*get_next_node(NodeT*now){
		if (now->right){
			return get_min(now->right);
		}
		else{
			while (now){
				if (is_left_child(now)){
					return now->parent;
				}
				else{
					now = now->parent;
				}
			}
			return 0; // its the end
		}
	}

	template<class NodeT>
	static NodeT*get_prev_node(NodeT*now){
		if (now->left){
			return get_max(now->left);
		}
		else{
			while (now){
				if (is_right_child(now)){
					return now->parent;
				}
				else{
					now = now->parent;
				}
			}
			return 0; // its the beginning
		}
	}


private:
	void remove(Node*node){
		// one leaf
		if (!node->left && !node->right){
			*get_parent_ptr(node) = 0;
			delete node;
		}
		// only one child
		else if (node->left && !node->right){
			*get_parent_ptr(node) = node->left;
			node->left->parent = node->parent;
			delete node;
		}
		else if (!node->left && node->right){
			*get_parent_ptr(node) = node->right;
			node->right->parent = node->parent;
			delete node;
		}
		// two children
		else{
			Node*other = get_prev_node(node);
			swap_nodes(node, other);
			remove(node);
		}
		assert(is_wellformed(root));
	}
public:
	bool remove(const T&value){
		Node*node = search(root, value);
		if (node){
			remove(node);
			return true;
		}
		else{
			return false;
		}
	}
};

// ------------------------------------------------------------------ //
//	Console
// ------------------------------------------------------------------ //

using namespace std;

namespace cmd{
	typedef void(*ptr)(Tree<int>&t, istream&in, ostream&out);

	void has(Tree<int>&t, istream&in, ostream&out){
		int value;
		if (in >> value){
			out << value << " is ";
			if (!t.contains(value)){
				out << "not ";
			}
			out << "in the tree.";
		}
		else{
			out << "Error : Could not parse the value";
		}
	}

	void add(Tree<int>&t, istream&in, ostream&out){
		int value;
		while (in >> value){
			if (!t.add(value)){
				out << "Error : " << value << " is already in the tree.\n";
			}
		}
		if (!in.eof()){
			out << "Error : Could not parse a value";
		}
	}

	void remove(Tree<int>&t, istream&in, ostream&out){
		int value;
		while (in >> value){
			if (!t.remove(value)){
				out << "Error : " << value << " was not in the tree.\n";
			}
		}
		if (!in.eof()){
			out << "Error : Could not parse a value";
		}
	}
}

void print_head(const Tree<int>&t, ostream&out){
	if (t.is_empty()){
		out << "The tree is empty." << endl;
	}
	else{
		t.dump(out);
	}

	out << "\n > ";
}

void print_tail(const Tree<int>&t, ostream&out){
	out << "\n=================================================================\n" << endl;
}

void run_terminal(const map<string, cmd::ptr>&cmd_map, istream&in, ostream&out){
	Tree<int>t;

	string line;

	print_head(t, out);
	while (getline(in, line)){
		istringstream cmd_in(line);
		string cmd_name;
		cmd_in >> cmd_name;
		out << '\n';

		if (cmd_name == "exit"){
			break;
		}

		map<string, cmd::ptr>::const_iterator found = cmd_map.find(cmd_name);
		if (found != cmd_map.end()){
			(*found->second)(t, cmd_in, out);

			char c;
			if (cmd_in >> c)
				out << "Error : additional parameters ignored" << endl;

			out << std::endl;
		}
		else{
			out << "Error : unknown command \"" << cmd_name << "\". Type \"help\" for a list of commands." << endl;
		}
		print_tail(t, out);
		print_head(t, out);
	}
}

int main(){
	try{
		srand(time(0));

		map<string, cmd::ptr>cmd_map;

		cmd_map["add"] = &cmd::add;
		cmd_map["has"] = &cmd::has;
		cmd_map["remove"] = &cmd::remove;

		run_terminal(cmd_map, cin, cout);
	}
	catch (std::exception&error){
		cerr << "Fatal error : Stopped on exception throw : " << error.what();
	}
	catch (...){
		cerr << "Fatal error : Stopped on unknown exception throw.";
	}
}