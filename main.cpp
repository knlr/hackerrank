//
//  main.cpp
//  hackerrrack
//
//  https://www.hackerrank.com/challenges/attribute-parser/problem
//
//  Created by Knut on 21/01/2024.
//

#include <iostream>
#include <map>
#include <memory>
#include <vector>
using namespace std;

#ifdef DEBUG
#define dcout(a) cout a
#else
#define dcout(a)
#endif



class Node
{
public:
    string name;
    map<string, string> values;
    vector<shared_ptr<Node>> children;
    weak_ptr<Node> parent;
    shared_ptr<Node> findChildNamed(string name);
};

shared_ptr<Node> Node::findChildNamed(string name)
{
    for (auto n : children)
        if (n->name == name)
            return n;
    return nullptr;
}


int main(int argc, const char * argv[]) {
    
    int n, q;
    dcout( << "number of hrml lines pls: ");
    cin >> n;
    dcout( << "number of queries pls: ");
    cin >> q;
    shared_ptr<Node> root = make_shared<Node>();
    auto cur = root;
    for (int i = 0; i < n + 1; i++)
    {
        dcout(<< "reading markup line no " << i + 1 << " of " << n << endl);

        string line;
        dcout( << "line " << i << "pls: " << endl);
        getline(cin, line);
        dcout( << "line read: '" << line << "'" << endl);
        if (line.substr(0, 2) == "</")
        {
            // closing tag. go up one level
            cur = cur->parent.lock();
            dcout(<< "closing tag found. " << "setting current node to " << cur->name << endl);
        }
        else if (line.substr(0, 1) == "<")
        {
            dcout( << "opening tag found" << endl);
            bool tagNameComplete = false;
            bool isAttributeName = false;
            bool isAttributeValue = false;
            bool tagClosed = false;
            string tagName;
            string attributeName;
            string attributeValue;
            map<string, string> attributes;
            for (const char c : line.substr(1, line.size()-1))
            {
                if (c == '>')
                {
                    tagNameComplete = true;
                    tagClosed = true;
                }
                if (!tagNameComplete)
                {
                    if (c == ' ')
                    {
                        tagNameComplete = true;
                        isAttributeName = true;
                        dcout( << "tag name complete: '" << tagName << "'" << endl);
                    }
                    else
                    {
                        tagName.append(string(1, c));
                        //dcout << "appending '" << c << "' to tag name" << endl;
                    }
                }
                else if (!tagClosed)
                {
                    if (c == ' ') continue;
                    if (isAttributeName)
                    {
                        if (c != '=')
                        {
                            attributeName.append(string(1, c));
                            //dcout( << "appending '" << c << "' to attrbiute name" << endl);
                        }
                        else 
                        {
                            isAttributeName = false;
                            isAttributeValue = true;
                            dcout( << "attribute name  '" << attributeName << "' found" << endl);
                        }
                    }
                    if (isAttributeValue)
                    {
                        if (c == '\"')
                        {
                            if (attributeValue.size() > 0)
                            {
                                isAttributeValue = false;
                                attributes[attributeName] = attributeValue;
                                dcout(<< "attribute value  '" << attributeValue << "' found" << endl);
                                attributeValue.clear();
                                attributeName.clear();
                                isAttributeName = true;
                            }
                        }
                        else if (c == '=')
                        {
                            continue;
                        }
                        else
                        {
                            attributeValue.append(string(1, c));
                            dcout( << "appending '" << c << "' to attrbiute value" << endl);
                        }
                    }
                }
            }
            shared_ptr<Node> newNode = make_shared<Node>();
            newNode->name = tagName;
            newNode->values = attributes;
            newNode->parent = cur;
            cur->children.push_back(newNode);
            dcout( << "new node '" << tagName << "' inserted under '" << cur->name << "'" << endl);
            cur = newNode;
        }
    }
    vector<string> queries;
    for (int i = 0; i < q; i++)
    {
        string query;
        dcout( << "query pls:");
        cin >> query;
        size_t delimiterPos = 0;
        size_t curPos = 0;
        cur = root;
        size_t tildePos = query.find("~", curPos);
        bool end = false;
        while (!end)
        {
            delimiterPos = query.find(".", curPos);
            if (delimiterPos == string::npos)
            {
                delimiterPos = tildePos;
                end = true;
            }
            auto tagName = query.substr(curPos, delimiterPos - curPos);
            cur = cur->findChildNamed(tagName);
            if (cur == nullptr)
            {
                cout << "Not Found!" << endl;
                end = true;
            }
            curPos = delimiterPos + 1;
        }
        if (cur == nullptr)
            continue;
        string attributeName = query.substr(tildePos + 1, query.length() - tildePos);
        auto it = cur->values.find(attributeName);
        if (it != cur->values.end())
        {
            cout << it->second << endl;
        }
        else
        {
            cout << "Not Found!" << endl;
        }
    }
    return 0;
}


