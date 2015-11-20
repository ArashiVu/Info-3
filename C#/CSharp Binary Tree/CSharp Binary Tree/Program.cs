using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CSharp_Binary_Tree
{
    class Node
    {
        public int data;
        public Node left, right;

        public Node(int data)
        {
            this.data = data;
            left = null;
            right = null;

        }
    }

    class BinaryTreeImp
    {
        Node root;
        static int count = 0;

        public BinaryTreeImp()
        {
            root = null;

        }
        public Node addNode(int data)
        {
            Node newNode = new Node(data);

            if (root == null)
            {
                root = newNode;

            }
            count++;
            return newNode;


        }

        public void insertNode(Node root, Node newNode)
        {
            Node temp;
            temp = root;

            if (newNode.data < temp.data)
            {
                if (temp.left == null)
                {
                    temp.left = newNode;

                }

                else
                {
                    temp = temp.left;
                    insertNode(temp, newNode);

                }
            }
            else if (newNode.data > temp.data)
            {
                if (temp.right == null)
                {
                    temp.right = newNode;

                }

                else
                {
                    temp = temp.right;
                    insertNode(temp, newNode);
                }
            }
        }


        public void displayTree(Node root)
        {
            Node temp;
            temp = root;

            if (temp == null)
                return;
            displayTree(temp.left);
            System.Console.Write(temp.data + " ");
            displayTree(temp.right);


        }


        public void has(int data)
        {
            
        }


        static void Main(string[] args)
        {
            BinaryTreeImp btObj = new BinaryTreeImp();
            Node iniRoot = btObj.addNode(5);


            btObj.insertNode(btObj.root, iniRoot);
            btObj.insertNode(btObj.root, btObj.addNode(666));
            btObj.insertNode(btObj.root, btObj.addNode(10));
            btObj.insertNode(btObj.root, btObj.addNode(2));
            btObj.insertNode(btObj.root, btObj.addNode(3));
            btObj.displayTree(btObj.root);

            System.Console.WriteLine("The sum of nodes are " + count);
            Console.ReadLine();

        }
    }
}
