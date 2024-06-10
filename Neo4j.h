#pragma once
#include"Linked List.h"
#include"Vector.h"
#include<string>
#include<fstream>
#include<utility>

struct Node
{
	string label;
	Vector<string> att;
	Vector<string> attName;
	Vector<string> dataType;

	bool operator==(Node temp)
	{
		if (label != temp.label)
			return false;
		if (attName.size() != temp.attName.size())
			return false;
		for (int i = 0; i < attName.size(); i++)
		{
			if (attName[i] != temp.attName[i])
				return false;
		}
		for (int i = 0; i < att.size(); i++)
		{
			if (att[i] != temp.att[i])
				return false;
		}
		return true;
	}
};

struct Edge
{
	Node from;
	Node to;
	string type;
	Vector<string> att;
	Vector<string> attName;
	Vector<string> dataType;
};

class graph 
{
private:
	LinkedList<Node> nodes;
	LinkedList<Edge> edges;

	// Read from file
	string whatIsDataType(string att)
	{
		string dtype;
		bool f = false;
		for (int i = 0; i < att.length(); i++)
		{
			if ((att[i] < '0' || att[i]>'9') && att[i] != '.')
			{
				dtype = "string";
				f = true;
				break;
			}
		}
		if (!f)
			dtype == "double";
		return dtype;
	}

	void readFromFile(string fileName, string _label)
	{
		string l = _label;
		ifstream rdr(fileName);
		string name;
		getline(rdr, name);
		Vector<string>Names;
		string n;
		for (int i = 0; i < name.length(); i++)
		{
			if (name[i] == ',')
			{
				Names.push_back(n);
				n.clear();
			}
			else
			{
				n += name[i];
			}
		}
		Names.push_back(n);
		int totalAtt = Names.size();
		Vector<string>dtype;
		while (!rdr.eof())
		{
			Node temp;
			temp.label = l;
			for (int i = 0; i < totalAtt; i++)
			{
				temp.attName.push_back(Names[i]);
			}
			name.clear();
			n.clear();
			getline(rdr, name);
			for (int i = 0; i < name.length(); i++)
			{
				if (name[i] == ',')
				{
					temp.att.push_back(n);
					n.clear();
				}
				else
				{
					n += name[i];
				}
			}
			temp.att.push_back(n);
			if (dtype.size() == 0)
			{
				for (int i = 0; i < totalAtt; i++)
				{
					dtype.push_back(whatIsDataType(temp.att[i]));
				}
			}
			for (int i = 0; i < totalAtt; i++)
			{
				temp.dataType.push_back(dtype[i]);
			}
			this->nodes.insert(temp);
		}
	}

	// Create querry functions
	bool itsCreateNode(string querry)
	{
		int count = 0;
		bool flag = false;
		while (querry[count] != ')')
		{
			if (querry[count] == '{')
			{
				flag = true;
				break;
			}
			count++;
		}
		return flag;
	}

	void retNodes(string querry, string& n1, string& n2)
	{
		n1.clear();
		n2.clear();
		bool flag = false;
		int count = 0;
		while (querry[count] != '-')
		{
			count++;
		}
		if (querry[count - 1] == '<')
			flag = true;
		if(flag)
		{
			for (int i = 9; querry[i] != ')'; i++)
			{
				n2.push_back(querry[i]);
			}
			while (querry[count] != '(')
			{
				count++;
			}
			count++;
			while (querry[count] != ')')
			{
				n1.push_back(querry[count]);
				count++;
			}
		}
		else
		{
			for (int i = 8; querry[i] != ')'; i++)
			{
				n1.push_back(querry[i]);
			}
			while (querry[count] != '(')
			{
				count++;
			}
			count++;
			while (querry[count] != ')')
			{
				n2.push_back(querry[count]);
				count++;
			}
		}
	}

	Node itsCreateNodeQuerry(string querry, string& name)
	{
		int count = 7;
		Node obj;
		if (querry[count] == '(')
			count++;
		name.clear();
		for (; querry[count] != ':'; count++)
		{
			name.push_back(querry[count]);
		}
		if (querry[count] == ':')
			count++;
		string label;
		for (; querry[count] != ' ' && querry[count] != ')' && querry[count] != '{'; count++)
		{
			label.push_back(querry[count]);
		}
		obj.label = label;
		if (querry[count] == ' ')
			count++;
		if (querry[count] == '{')
		{
			count++;
			for (; querry[count] != '}';)
			{
				label.clear();
				for (; querry[count] != ':'; count++)
				{
					label.push_back(querry[count]);
				}
				obj.attName.push_back(label);
				if (querry[count] == ':')
					count++;
				if (querry[count] == ' ')
					count++;
				bool isString = false;
				label.clear();
				for (; querry[count] != ',' && querry[count] != '}'; count++)
				{
					if (querry[count] == '\"')
					{
						isString = true;
						continue;
					}
					label.push_back(querry[count]);
				}
				obj.att.push_back(label);
				if (isString)
					obj.dataType.push_back("string");
				else
					obj.dataType.push_back("double");
				if (querry[count] == ',')
					count++;
				if (querry[count] == ' ')
					count++;
			}
		}
		if (querry[count] == ')')
			count++;
		return obj;
	}

	Edge itsCreateEdgeQuerry(string querry, Node n1, Node n2, string& n)
	{
		Edge obj;
		obj.from = n1;
		obj.to = n2;
		string temp;
		int count = 0;
		while (querry[count] != '[')
		{
			count++;
		}
		count++;
		n.clear();
		while (querry[count] != ':')
		{
			n.push_back(querry[count]);
			count++;
		}
		if (querry[count] == ':')
			count++;
		if (querry[count] == ' ')
			count++;
		while (querry[count] != ' ' && querry[count] != '{' && querry[count] != ']')
		{
			temp.push_back(querry[count]);
			count++;
		}
		obj.type = temp;
		temp.clear();
		if (querry[count] == ' ')
			count++;
		if (querry[count] == '{')
		{
			count++;
			for (; querry[count] != '}';)
			{
				temp.clear();
				for (; querry[count] != ':'; count++)
				{
					temp.push_back(querry[count]);
				}
				obj.attName.push_back(temp);
				if (querry[count] == ':')
					count++;
				if (querry[count] == ' ')
					count++;
				bool isString = false;
				temp.clear();
				for (; querry[count] != ',' && querry[count] != '}'; count++)
				{
					if (querry[count] == '\"')
					{
						isString = true;
						continue;
					}
					temp.push_back(querry[count]);
				}
				obj.att.push_back(temp);
				if (isString)
					obj.dataType.push_back("string");
				else
					obj.dataType.push_back("double");
				if (querry[count] == ',')
					count++;
				if (querry[count] == ' ')
					count++;
			}
		}
		return obj;
	}

	// Match querry functions
	bool matchCheckForProperties(string querry)
	{
		bool flag = false;
		int i = 0;
		while (querry[i] != ')')
		{
			if (querry[i] == '{')
			{
				flag = true;
				break;
			}
			i++;
		}
		return flag;
	}

	Vector<Node> matchWithNoRelation(string querry, string& name)
	{
		Vector<Node> obj;
		int count = 0;
		name.clear();
		while (querry[count] != '(')
		{
			count++;
		}
		count++;
		while (querry[count] != ':')
		{
			name.push_back(querry[count]);
			count++;
		}
		count++;
		if (querry[count] == ' ')
			count++;
		string label;
		bool flag = matchCheckForProperties(querry);
		Vector<string> att;
		Vector<string> attName;
		Vector<string> dtype;
		while (querry[count] != ')' && querry[count] != ' ' && querry[count] != '{')
		{
			label.push_back(querry[count]);
			count++;
		}
		if (flag)
		{
			if (querry[count] == ' ')
				count++;
			if (querry[count] == '{')
				count++;
			while (querry[count] != '}')
			{
				if (querry[count] == ' ')
					count++;
				string temp;
				while (querry[count] != ':' && querry[count] != ' ')
				{
					temp.push_back(querry[count]);
					count++;
				}
				attName.push_back(temp);
				if (querry[count] == ':' || querry[count] == ' ')
					count++;
				if (querry[count] == ':' || querry[count] == ' ')
					count++;
				bool isString = false;
				temp.clear();
				while (querry[count] != '}' && querry[count] != ' ' && querry[count] != ',')
				{
					if (querry[count] == '\"')
					{
						isString = true;
						count++;
						continue;
					}
					temp.push_back(querry[count]);
					count++;
				}
				att.push_back(temp);
				if (isString)
				{
					dtype.push_back("string");
				}
				else
					dtype.push_back("double");
				if (querry[count] == ',' || querry[count] == ' ')
					count++;
				if (querry[count] == ',' || querry[count] == ' ')
					count++;
			}
		}

		for (int i = 0; i < nodes.size(); i++)
		{
			if (nodes[i].label == label)
			{
				if (flag)
				{
					for (int j = 0; j < attName.size(); j++)
					{
						for (int k = 0; k < nodes[i].attName.size(); k++)
						{
							if (attName[j] == nodes[i].attName[k])
							{
								if (att[j] == nodes[i].att[k])
								{
									obj.push_back(nodes[i]);
								}
								break;
							}
						}
					}
				}
				else
				{
					obj.push_back(nodes[i]);
				}
			}
		}
		return obj;
	}

	bool checkWithMatchRelationProperties(string querry, int count)
	{
		bool flag = false;
		while (querry[count] != ')' && querry[count] != ']')
		{
			if (querry[count] == '{')
			{
				flag = true;
				break;
			}
			count++;
		}
		return flag;
	}

	string getName(string querry, int& count)
	{
		string name;
		
		while (querry[count] != ':')
		{
			name.push_back(querry[count]);
			count++;
		}
		count++;
		return name;
	}

	string getLabel(string querry, int& count)
	{
		string label;
		if (querry[count] == ' ')
			count++;
		while (querry[count] != ')' && querry[count] != ' ' && querry[count] != '{' && querry[count] != ']')
		{
			label.push_back(querry[count]);
			count++;
		}
		return label;
	}

	void retriveAtt(string querry, int count, Vector<string>& attName, Vector<string>& att)
	{
		while (querry[count] != '{')
		{
			count++;
		}
		count++;
		while (querry[count] != '}')
		{
			if (querry[count] == ' ')
				count++;
			string name;
			while (querry[count] != ':')
			{
				name.push_back(querry[count]);
				count++;
			}
			attName.push_back(name);
			name.clear();
			count++;
			if (querry[count] == ' ')
				count++;
			while (querry[count] != '}' && querry[count] != ' ' && querry[count] != ',')
			{
				if (querry[count]=='\"')
				{
					count++;
					continue;
				}
				name.push_back(querry[count]);
				count++;
			}
			att.push_back(name);
			if (querry[count] == ' ' || querry[count] == ',')
				count++;
			if (querry[count] == ' ' || querry[count] == ',')
				count++;
		}
	}

	Vector<Edge> matchWithRelation(string querry, string& name1, string& name2, string& name3)
	{
		int count = 0;
		string label1, label2, type;
		Vector<string> att1;
		Vector<string> attName1;
		Vector<string> att2;
		Vector<string> attName2;
		Vector<string> att3;
		Vector<string> attName3;
		Vector<Edge> obj;
		while (querry[count] != '(')
		{
			count++;
		}
		count++;
		name1 = getName(querry, count);
		label1 = getLabel(querry, count);
		int lcount = count;
		while (querry[lcount] != '>')
		{
			lcount++;
		}
		lcount++;
		if (querry[lcount] == ' ' || querry[lcount] == '(')
			lcount++;
		if (querry[lcount] == ' ' || querry[lcount] == '(')
			lcount++;
		name2 = getName(querry, lcount);
		label2 = getLabel(querry, lcount);
		int rcount = count;
		while (querry[rcount] != '[')
		{
			rcount++;
		}
		rcount++;
		name3 = getName(querry, rcount);
		type = getLabel(querry, rcount);
		for (int i = 0; i < edges.size(); i++)
		{
			if (edges[i].type == type)
			{
				if (checkWithMatchRelationProperties(querry, rcount))
				{
					retriveAtt(querry, rcount, attName3, att3);
					bool* arr = new bool[attName3.size()] {};
					for (int j = 0; j < attName3.size(); j++)
					{
						for (int k = 0; k < edges[i].attName.size(); k++)
						{
							if (attName3[j] == edges[i].attName[k])
							{
								if (att3[j] == edges[i].att[k])
								{
									arr[j] = true;
								}
								break;
							}
						}
					}
					bool f = false;
					for (int j = 0; j < attName3.size(); j++)
					{
						if (!arr[j])
						{
							f = true;
							break;
						}
					}
					if (f)
						continue;
				}
			}
			else
				continue;
			if (edges[i].from.label == label1)
			{
				if (checkWithMatchRelationProperties(querry, count))
				{
					retriveAtt(querry, count, attName1, att1);
					bool* arr = new bool[attName1.size()] {};
					for (int j = 0; j < attName1.size(); j++)
					{
						for (int k = 0; k < edges[i].from.attName.size(); k++)
						{
							if (attName1[j] == edges[i].from.attName[k])
							{
								if (att1[j] == edges[i].from.att[k])
								{
									arr[j] = true;
								}
								break;
							}
						}
					}
					bool f = false;
					for (int j = 0; j < attName1.size(); j++)
					{
						if (!arr[j])
						{
							f = true;
							break;
						}
					}
					if (f)
						continue;
				}
			}
			else
				continue;
			if (edges[i].to.label == label2)
			{
				if (checkWithMatchRelationProperties(querry, lcount))
				{
					retriveAtt(querry, lcount, attName2, att2);;
					bool* arr = new bool[attName2.size()] {};
					for (int j = 0; j < attName2.size(); j++)
					{
						for (int k = 0; k < edges[i].to.attName.size(); k++)
						{
							if (attName2[j] == edges[i].to.attName[k])
							{
								if (att2[j] == edges[i].to.att[k])
								{
									arr[j] = true;
								}
								break;
							}
						}
					}
					bool f = false;
					for (int j = 0; j < attName2.size(); j++)
					{
						if (!arr[j])
						{
							f = true;
							break;
						}
					}
					if (f)
						continue;
				}
			}
			else
				continue;
			obj.push_back(edges[i]);
		}
		return obj;
	}

	//Return querry functions
	Vector<string> getVariable(string querry)
	{
		int count = 0;
		Vector<string> obj;
		string name;
		while (querry[count] != ' ')
		{
			count++;
		}
		count++;
		while (querry[count] != '\0')
		{
			while (querry[count] != ',' && querry[count] != ' ' && querry[count] != '\0')
			{
				name.push_back(querry[count]);
				count++;
			}
			obj.push_back(name);
			name.clear();
			if (querry[count] == ' ' || querry[count] == ',')
				count++;
			if (querry[count] == ' ' || querry[count] == ',')
				count++;
		}
		return obj;
	}

	void getVariable(string querry, Vector<string>& names, Vector<string>& properties, Vector<string>& alias)
	{
		int count = 0;
		while (querry[count] != ' ')
		{
			count++;
		}
		count++;
		string temp;
		while (querry[count] != '\0')
		{
			if (querry[count] == ' ')
				count++;
			if (querry[count] == ',')
				count++;
			while (querry[count] != '.')
			{
				temp.push_back(querry[count]);
				count++;
			}
			names.push_back(temp);
			temp.clear();
			count++;
			while (querry[count] != ' ' && querry[count] != ',' && querry[count] != '\0')
			{
				temp.push_back(querry[count]);
				count++;
			}
			properties.push_back(temp);
			temp.clear();
			if (querry[count] != '\0')
			{
				count++;
				if (querry[count] == 'a' && querry[count + 1] == 's' && querry[count + 2] == ' ')
				{
					count++;
					count++;
					count++;
					while (querry[count] != ',' && querry[count] != ' ' && querry[count] != '\0')
					{
						temp.push_back(querry[count]);
						count++;
					}
					alias.push_back(temp);
					temp.clear();
				}
				else
				{
					alias.push_back(names[names.size() - 1] + properties[properties.size() - 1]);
				}
			}
			else
			{
				alias.push_back(names[names.size() - 1] + properties[properties.size() - 1]);
			}
			
		}
	}

	//Where querry functions
	void getEverything(string querry, Vector<string>& name, Vector<string>& att, Vector<string>& opr, Vector<string>& val)
	{
		int count = 6;
		while (querry[count] != '\0')
		{
			string temp;
			if (querry[count] == ' ')
				count++;
			if (querry[count] == 'A' || querry[count] == '&')
			{
				if (querry[count] == '&')
				{
					count++;
				}
				else
				{
					if (querry[count + 1] == 'N')
					{
						if (querry[count + 2] == 'D')
						{
							count = count + 2;
						}
					}
				}
			}
			if (querry[count] == 'O' || querry[count] == '|')
			{
				if (querry[count] == '|')
				{
					count++;
				}
				else
				{
					if (querry[count + 1] == 'R')
					{
						count = count + 1;
					}
				}
			}
			if (querry[count] == 'N' || querry[count] == '!')
			{
				if (querry[count] == '!')
				{
					count++;
				}
				else
				{
					if (querry[count + 1] == 'O')
					{
						if (querry[count + 2] == 'T')
						{
							count = count + 2;
						}
					}
				}
			}
			while (querry[count] != '.')
			{
				temp.push_back(querry[count]);
				count++;
			}
			name.push_back(temp);
			temp.clear();
			count++;
			while (querry[count] != ' ' && querry[count] != '&')
			{
				temp.push_back(querry[count]);
				count++;
			}
			att.push_back(temp);
			temp.clear();
			if (querry[count] == ' ' || querry[count] == '&')
				count++;
			if (querry[count] == ' ' || querry[count] == '&')
				count++;
			while (querry[count] != ' ')
			{
				temp.push_back(querry[count]);
				count++;
			}
			opr.push_back(temp);
			temp.clear();
			count++;
			while (querry[count] != ' ' && querry[count] != '\0')
			{
				temp.push_back(querry[count]);
				count++;
			}
			val.push_back(temp);
			temp.clear();
		}
	}

	//Merge querry functions
	void getThings(string querry, string& name, string& label, Vector<string>& attName, Vector<string>& att)
	{
		int count = 6;
		while (querry[count] != '(')
		{
			count++;
		}
		count++;
		if (querry[count] == ' ')
			count++;
		while (querry[count] != ':')
		{
			name.push_back(querry[count]);
			count++;
		}
		count++;
		while (querry[count] != ' ' && querry[count] != '{')
		{
			label.push_back(querry[count]);
			count++;
		}
		if (querry[count] == ' ' || querry[count] == '{')
			count++;
		if (querry[count] == ' ' || querry[count] == '{')
			count++;
		if (querry[count] == ' ' || querry[count] == '{')
			count++;
		while (querry[count] != '}')
		{
			if (querry[count] == ' ' || querry[count] == ',')
				count++;
			string temp;
			while (querry[count] != ':')
			{
				temp.push_back(querry[count]);
				count++;
			}
			count++;
			attName.push_back(temp);
			temp.clear();
			while (querry[count] != ',' && querry[count] != ' ' && querry[count] != '}')
			{
				if (querry[count] == '\"')
				{
					count++;
					continue;
				}
				temp.push_back(querry[count]);
				count++;
			}
			att.push_back(temp);
		}
	}

	void getNewThings(string querry, int count, Vector<string>& attName, Vector<string>& att)
	{
		while (querry[count] != '\0')
		{
			string temp;
			while (querry[count] != '.')
				count++;
			count++;
			while (querry[count] != ' ' && querry[count] != '=')
			{
				temp.push_back(querry[count]);
				count++;
			}
			attName.push_back(temp);
			temp.clear();
			if (querry[count] == ' ' || querry[count] == '=')
				count++;
			if (querry[count] == ' ' || querry[count] == '=')
				count++;
			while (querry[count] != ',' && querry[count] != '\0')
			{
				if (querry[count] == '\"')
				{
					count++;
					continue;
				}
				temp.push_back(querry[count]);
				count++;
			}
			att.push_back(temp);
			if (querry[count] == ',')
				count++;
		}
	}

	//Save querry function
	void save(string fname)
	{
		ofstream store(fname);
		if (!store.is_open())
		{
			cout << "File did'nt open";
			return;
		}
		store << "Nodes\n";
		for (int i = 0; i < nodes.size(); i++)
		{
			store << nodes[i].label << " {";
			for (int j = 0; j < nodes[i].attName.size(); j++)
			{
				store << nodes[i].attName[j] << ": " << nodes[i].att[j];
				if (j < nodes[i].attName.size() - 1)
					store << ", ";
			}
			store << "}\n";
		}
		store << "Edges\n";
		for (int i = 0; i < edges.size(); i++)
		{
			/*store << edges[i].from.label << "{ ";
			for (int j = 0; j < edges[i].from.attName.size(); j++)
			{
				store << edges[i].from.attName[j] << ": " << edges[i].from.att[j];
				if (j < edges[i].from.attName.size() - 1)
					store << ", ";
			}
			store << "} [";*/
			int a, b;
			for (int j = 0; j < nodes.size(); j++)
			{
				if (nodes[j] == edges[i].from)
				{
					a = j;
				}
				if (nodes[j] == edges[i].to)
				{
					b = j;
				}
			}
			store << a << " ";
			store << "[";
			store << edges[i].type << " {";
			for (int j = 0; j < edges[i].attName.size(); j++)
			{
				store << edges[i].attName[j] << ": " << edges[i].att[j];
				if (j < edges[i].attName.size() - 1)
					store << ", ";
			}
			store << "}] ";
			store << b << "\n";
			/*store << edges[i].to.label << "{ ";
			for (int j = 0; j < edges[i].to.attName.size(); j++)
			{
				store << edges[i].to.attName[j] << ": " << edges[i].to.att[j];
				if (j < edges[i].to.attName.size() - 1)
					store << ", ";
			}
			store << "}\n";*/
		}
	}

	//Load querry function
	void load(string fname)
	{
		ifstream rdr;
		rdr.open(fname);
		if (!rdr.is_open())
		{
			cout << "file did'nt open";
			return;
		}
		string temp;
		getline(rdr, temp);
		if (temp == "Nodes")
			temp.clear();
		while (!rdr.eof())
		{
			getline(rdr, temp);
			while (temp != "Edges")
			{
				Node obj;
				int count = 0;
				string label;
				while (temp[count] != ' ')
				{
					label.push_back(temp[count]);
					count++;
				}
				obj.label = label;
				label.clear();
				count++;
				count++;
				while (temp[count] != '}')
				{
					if (temp[count] == ' ')
						count++;
					while (temp[count] != ':')
					{
						label.push_back(temp[count]);
						count++;
					}
					obj.attName.push_back(label);
					label.clear();
					count++;
					if (temp[count] == ' ')
						count++;
					while (temp[count] != ',' && temp[count] != '}')
					{
						label.push_back(temp[count]);
						count++;
					}
					obj.att.push_back(label);
					label.clear();
					if (temp[count] == ',')
						count++;
				}
				nodes.insert(obj);
				temp.clear();
				getline(rdr, temp);
			}
			temp.clear();
			getline(rdr, temp);
			while (!rdr.eof())
			{
				int count = 2;
				Edge obj;
				int in;
				string type;
				in = temp[0] - '0';
				obj.from = nodes[in];
				while (temp[count] != ' ')
				{
					type.push_back(temp[count]);
					count++;
				}
				obj.type = type;
				type.clear();
				count++;
				count++;
				while (temp[count] != '}')
				{
					while (temp[count] != ':')
					{
						type.push_back(temp[count]);
						count++;
					}
					obj.attName.push_back(type);
					type.clear();
					count++;
					while (temp[count] != ',' && temp[count] != '}')
					{
						type.push_back(temp[count]);
						count++;
					}
					obj.att.push_back(type);
					if (temp[count] == ',')
						count++;
				}
				count++;
				count++;
				count++;
				in = temp[count] - '0';
				obj.to = nodes[in];
				edges.insert(obj);
				temp.clear();
				getline(rdr, temp);
			}
		}
	}

	//Querry Processor
	void processQuerry(Vector<string> querry, Vector<pair<Node, string>>& resNode, Vector<pair<Vector<Node>, string>>& resNodes, Vector <pair<Edge, string>>& resEdge, Vector<pair<Vector<Edge>, string>>& resEdges, Vector<pair<string, string>>& resSpecific, Vector<pair<Vector<string>, string>>& resSpecifics)
	{
		Vector<Vector<Node>> match;
		Vector<string> matchStr;
		Vector<Node> allNodes;
		Vector<string> allNodesStr;
		Vector<Vector<Edge>> matchEdge;
		Vector<pair<pair<string, string>, string>> matchEdgeStr;
		Vector<Edge> allEdges;
		Vector<string> allEdgesStr;
		for (int index = 0; index < querry.size(); index++)
		{
			string s = querry[index];
			string kw;
			for (int j = 0; s[j] != ' '; j++)
			{
				kw.push_back(s[j]);
			}
			if (kw == "CREATE")
			{
				if (itsCreateNode(s))
				{
					string name;
					Node temp = itsCreateNodeQuerry(s, name);
					nodes.insert(temp);
					allNodes.push_back(temp);
					allNodesStr.push_back(name);
				}
				else
				{
					string n1, n2;
					retNodes(s, n1, n2);
					Node no1, no2;
					bool f1 = false, f2 = false;
					for (int i = 0; i < allNodesStr.size(); i++)
					{
						if (allNodesStr[i] == n1)
						{
							no1 = allNodes[i];
							f1 = true;
						}
						if (allNodesStr[i] == n2)
						{
							no2 = allNodes[i];
							f2 = true;
						}
						if (f1 && f2)
						{
							break;
						}
					}
					string name;
					Edge temp = itsCreateEdgeQuerry(s, no1, no2, name);
					edges.insert(temp);
					allEdges.push_back(temp);
					allEdgesStr.push_back(name);
				}
			}
			else if (kw == "MATCH")
			{
				int count = 0;
				bool type1 = false;
				while (s[count] != '\0')
				{
					if (s[count] == ':')
					{
						type1 = true;
						break;
					}
					count++;
				}
				if (!type1)
				{
					string name;
					count = 5;
					Vector<Node> temp;
					while (s[count] != '(')
					{
						count++;
					}
					count++;
					while (s[count] != ')')
					{
						name.push_back(s[count]);
						count++;
					}
					for (int i = 0; i < nodes.size(); i++)
					{
						temp.push_back(nodes[i]);
					}
					matchStr.push_back(name);
					match.push_back(temp);
				}
				else
				{
					bool isTheirARelation = false;
					count = 5;
					while (s[count] != '\0')
					{
						if (s[count] == '-')
						{
							isTheirARelation = true;
							break;
						}
						count++;
					}
					if (!isTheirARelation)
					{
						string name;
						Vector<Node>temp = matchWithNoRelation(s, name);
						match.push_back(temp);
						matchStr.push_back(name);
					}
					else
					{
						string name1, name2, name3;
						Vector<Edge>temp = matchWithRelation(s, name1, name2, name3);
						matchEdge.push_back(temp);
						matchEdgeStr.push_back({ {name1,name2},name3 });
					}
				}
			}
			else if (kw == "RETURN")
			{
				int count = 0;
				bool specificRet = false;
				while (s[count] != '\0')
				{
					if (s[count] == '.')
					{
						specificRet = true;
						break;
					}
					count++;
				}
				if (!specificRet)
				{
					Vector<string>names = getVariable(s);
					bool found = false;
					for (int i = 0; i < names.size(); i++)
					{
						found = false;
						for (int j = 0; j < allNodesStr.size(); i++)
						{
							if (allNodesStr[j] == names[i])
							{
								found = true;
								resNode.push_back({ allNodes[j],names[i] });
								break;
							}
						}
						if (found)
							continue;
						for (int j = 0; j < matchStr.size(); j++)
						{
							if (matchStr[j] == names[i])
							{
								found = true;
								pair<Vector<Node>, string>p;
								p.first = match[j];
								p.second = names[i];
								resNodes.push_back(p);
								break;
							}
						}
						if (found)
							continue;
						for (int j = 0; j < allEdgesStr.size(); j++)
						{
							if (allEdgesStr[j] == names[i])
							{
								found = true;
								resEdge.push_back({ allEdges[j],names[i] });
								break;
							}
						}
						if (found)
							continue;
						for (int j = 0; j < matchEdgeStr.size(); j++)
						{
							if (matchEdgeStr[j].first.first == names[i])
							{
								Vector<Node>temp;
								for (int k = 0; k < matchEdge[j].size(); k++)
								{
									temp.push_back(matchEdge[j][k].from);
								}
								found = true;
								resNodes.push_back({ temp,names[i] });
								break;
							}
							if (found)
								break;
							if (matchEdgeStr[j].first.second == names[i])
							{
								Vector<Node>temp;
								for (int k = 0; k < matchEdge[j].size(); k++)
								{
									temp.push_back(matchEdge[j][k].to);
								}
								found = true;
								resNodes.push_back({ temp,names[i] });
								break;
							}
							if (found)
								break;
							if (matchEdgeStr[j].second == names[i])
							{
								found = true;
								resEdges.push_back({ matchEdge[j],names[i] });
								break;
							}
						}
						if (found)
							continue;
					}
				}
				else
				{
					bool found = false;
					Vector<string>names, properties, alias;
					getVariable(s, names, properties, alias);
					for (int i = 0; i < names.size(); i++)
					{
						found = false;
						for (int j = 0; j < allNodesStr.size(); j++)
						{
							if (allNodesStr[j] == names[i])
							{
								for (int k = 0; k < allNodes[j].attName.size(); j++)
								{
									if (allNodes[j].attName[k] == properties[i])
									{
										found = true;
										resSpecific.push_back({ allNodes[j].att[k],alias[i] });
										break;
									}
								}
								break;
							}
						}
						if (found)
							continue;
						for (int j = 0; j < matchStr.size(); j++)
						{
							if (matchStr[j] == names[i])
							{
								int in = -1;
								Vector<string>temp;
								for (int k = 0; k < match[j].size(); k++)
								{
									if (in == -1)
									{
										for (int l = 0; l < match[j][k].attName.size(); i++)
										{
											if (match[j][k].attName[l] == properties[i])
											{
												in = l;
												break;
											}
										}
									}
									found = true;
									temp.push_back(match[j][k].att[in]);
								}
								pair<Vector<string>, string>p;
								p.first = temp;
								p.second = alias[i];
								resSpecifics.push_back(p);
								break;
							}
						}
						if (found)
							continue;
						for (int j = 0; j < allEdgesStr.size(); j++)
						{
							if (allEdgesStr[j] == names[i])
							{
								for (int k = 0; k < allEdges[j].attName.size(); k++)
								{
									if (allEdges[j].attName[k] == properties[i])
									{
										found = true;
										resSpecific.push_back({ allEdges[j].att[k],alias[i] });
										break;
									}
								}
								break;
							}
						}
						if (found)
							continue;
						for (int j = 0; j < matchEdgeStr.size(); j++)
						{
							if (matchEdgeStr[j].first.first == names[i])
							{
								int in = -1;
								Vector<string>temp;
								for (int k = 0; k < matchEdge[j].size(); k++)
								{
									if (in == -1)
									{
										for (int l = 0; l < matchEdge[j][k].from.attName.size(); i++)
										{
											if (matchEdge[j][k].from.attName[l] == properties[i])
											{
												in = l;
												break;
											}
										}
									}
									found = true;
									temp.push_back(matchEdge[j][k].from.att[in]);
								}
								resSpecifics.push_back({ temp,alias[i] });
								break;
							}
							if (matchEdgeStr[j].first.second == names[i])
							{
								int in = -1;
								Vector<string>temp;
								for (int k = 0; k < matchEdge[j].size(); k++)
								{
									if (in == -1)
									{
										for (int l = 0; l < matchEdge[j][k].to.attName.size(); i++)
										{
											if (matchEdge[j][k].to.attName[l] == properties[i])
											{
												in = l;
												break;
											}
										}
									}
									found = true;
									temp.push_back(matchEdge[j][k].to.att[in]);
								}
								resSpecifics.push_back({ temp,alias[i] });
								break;
							}
							if (matchEdgeStr[j].second == names[i])
							{
								int in = -1;
								Vector<string>temp;
								for (int k = 0; k < matchEdge[j].size(); k++)
								{
									if (in == -1)
									{
										for (int l = 0; l < matchEdge[j][k].attName.size(); i++)
										{
											if (matchEdge[j][k].attName[l] == properties[i])
											{
												in = l;
												break;
											}
										}
									}
									found = true;
									temp.push_back(matchEdge[j][k].att[in]);
								}
								resSpecifics.push_back({ temp,alias[i] });
								break;
							}
						}
					}
				}
			}
			else if (kw == "WHERE")
			{
				int count = 0;
				bool isAND = false;
				bool isOR = false;
				bool isNOT = false;
				bool nothing = false;
				while (s[count] != '\0')
				{
					if (s[count] == 'A' || s[count] == '&')
					{
						if (s[count] == '&')
						{
							isAND = true;
							break;
						}
						count++;
						if (s[count] == 'N')
						{
							count++;
							if (s[count] == 'D')
							{
								count++;
								if (s[count] == ' ')
								{
									isAND = true;
									break;
								}
							}
						}
						count++;
						continue;
					}
					if (s[count] == 'O' || s[count] == '|')
					{
						if (s[count] == '|')
						{
							isOR = true;
							break;
						}
						count++;
						if (s[count] == 'R')
						{
							count++;
							if (s[count] == ' ')
							{
								isOR = true;
								break;
							}
							continue;
						}
						continue;
					}
					if (s[count] == 'N' || s[count] == '!')
					{
						if (s[count] == '!')
						{
							isNOT = true;
							break;
						}
						count++;
						if (s[count] == 'O')
						{
							count++;
							if (s[count] == 'T')
							{
								count++;
								if (s[count] == ' ')
								{
									isNOT = true;
									break;
								}
							}
						}
						count++;
						continue;
					}
					count++;
				}
				Vector<string>name;
				Vector<string>att;
				Vector<string>opr;
				Vector<string>val;
				Vector<Node> nMatch;
				if (isAND)
				{
					getEverything(s, name, att, opr, val);
					for (int i = 0; i < matchStr.size(); i++)
					{
						if (name[0] == matchStr[i])
						{
							for (int j = 0; j < match[i].size(); j++)
							{
								for (int k = 0; k < match[i][j].attName.size(); k++)
								{
									if (match[i][j].attName[k] == att[0])
									{

									}
								}
							}
						}
					}
				}
			}
			else if (kw == "MERGE")
			{
				int count = 5;
				bool doseNodeExist = false;
				string name, label;
				int num;
				Vector<string>attName, att;
				getThings(s, name, label, attName, att);
				for (int i = 0; i < nodes.size(); i++)
				{
					if (nodes[i].label == label)
					{
						for (int j = 0; j < attName.size(); j++)
						{
							bool f = false;
							for (int k = 0; k < nodes[i].attName.size(); k++)
							{
								if (attName[j] == nodes[i].attName[k])
								{
									if (att[j] == nodes[i].att[k])
									{
										f = true;
										num = i;
									}
									break;
								}
							}
							if (f)
								doseNodeExist = true;
							else
								doseNodeExist = false;
						}
						if (doseNodeExist)
							break;
					}
				}
				if (!doseNodeExist)
				{
					index++;
					s = querry[index];
					int ccount = 13;
					if (s[ccount] == ' ')
						ccount++;
					getNewThings(s, ccount, attName, att);
					Node obj;
					obj.label = label;
					for (int i = 0; i < attName.size(); i++)
					{
						obj.attName.push_back(attName[i]);
						obj.att.push_back(att[i]);
					}
					nodes.insert(obj);
					allNodes.push_back(obj);
					allNodesStr.push_back(name);
				}
				else
				{
					index++;
					index++;
					s = querry[index];
					int ccount = 10;
					if (s[ccount] == ' ')
						ccount++;
					Vector<string>nAttName;
					Vector<string>nAtt;
					getNewThings(s, ccount, nAttName, nAtt);
					bool found1 = false;
					bool found2 = false;
					int rec;
					for (int i = 0; i < edges.size(); i++)
					{
						if (edges[i].from == nodes[num])
						{
							found1 = true;
							rec = i;
						}
						if (edges[i].to == nodes[num])
						{
							found2 = true;
							rec = i;
						}
						if (found1 || found2)
							break;
					}
					for (int i = 0; i < nAttName.size(); i++)
					{
						for (int j = 0; j < nodes[num].attName.size(); j++)
						{
							if (nodes[num].attName[j] == nAttName[i])
							{
								nodes[num].att[j] = nAtt[i];
							}
						}
					}
					if (found1)
					{
						edges[rec].from = nodes[num];
					}
					if (found2)
					{
						edges[rec].to = nodes[num];
					}
					allNodes.push_back(nodes[num]);
					allNodesStr.push_back(name);
				}
			}
			else if (kw == "SAVE")
			{
				int count = 5;
				string fname;
				while (s[count] != '(')
					count++;
				count++;
				while (s[count] != ')')
				{
					fname.push_back(s[count]);
					count++;
				}
				save(fname);
			}
			else if (kw == "LOAD")
			{
				int count = 5;
				string fname;
				while (s[count] != '(')
					count++;
				count++;
				while (s[count] != ')')
				{
					fname.push_back(s[count]);
					count++;
				}
				load(fname);
			}
			else if (kw == "READ")
			{
				int count = 5;
				while (s[count] != '(')
					count++;
				count++;
				string fname, label;
				while (s[count] != ',')
				{
					fname.push_back(s[count]);
					count++;
				}
				count++;
				count++;
				while (s[count] != ')')
				{
					label.push_back(s[count]);
					count++;
				}
				readFromFile(fname, label);
			}
		}
	}

public:


	void processor(Vector<string>querry)
	{
		Vector<pair<Node, string>> resNode;
		Vector<pair<Vector<Node>, string>> resNodes;
		Vector<pair<Edge, string>> resEdge;
		Vector<pair<Vector<Edge>, string>> resEdges;
		Vector<pair<string, string>> resSpecific;
		Vector<pair<Vector<string>, string>> resSpecifics;
		processQuerry(querry, resNode, resNodes, resEdge, resEdges, resSpecific, resSpecifics);
		for (int i = 0; i < resNode.size(); i++)
		{
			cout << "\n" << resNode[i].second << ": \n";
			cout << "Label: " << resNode[i].first.label << endl;
			for (int j = 0; j < resNode[i].first.attName.size(); j++)
			{
				cout << resNode[i].first.attName[j] << ": " << resNode[i].first.att[j] << "\t";
			}
			cout << endl;
		}
		for (int i = 0; i < resNodes.size(); i++)
		{
			cout << "\n" << resNodes[i].second << ": \n";
			for (int j = 0; j < resNodes[i].first.size(); j++)
			{
				cout << "Label: " << resNodes[i].first[j].label << endl;
				for (int k = 0; k < resNodes[i].first[j].attName.size(); k++)
				{
					cout << resNodes[i].first[j].attName[k] << ": " << resNodes[i].first[j].att[k] << "\t";
				}
				cout << endl;
			}
		}
		for (int i = 0; i < resEdge.size(); i++)
		{
			cout << "\n" << resEdge[i].second << ": \n";
			cout << "\nFrom: \n";
			cout << "Label: " << resEdge[i].first.from.label << endl;
			for (int k = 0; k < resEdge[i].first.from.attName.size(); k++)
			{
				cout << resEdge[i].first.from.attName[k] << ": " << resEdge[i].first.from.att[k] << "\t";
			}
			cout << endl;
			cout << "Relation Type: " << resEdge[i].first.type << endl;;
			for (int j = 0; j < resEdge[i].first.attName.size(); j++)
			{
				cout << resEdge[i].first.attName[j] << ": " << resEdge[i].first.att[j] << "\t";
			}
			cout << endl;
			cout << "\To: \n";
			cout << "Label: " << resEdge[i].first.to.label << endl;
			for (int k = 0; k < resEdge[i].first.to.attName.size(); k++)
			{
				cout << resEdge[i].first.to.attName[k] << ": " << resEdge[i].first.to.att[k] << "\t";
			}
			cout << endl;
		}
		for (int i = 0; i < resEdges.size(); i++)
		{
			cout << "\n" << resEdges[i].second << ": \n";
			for (int j = 0; j < resEdges[i].first.size(); j++)
			{
				cout << "\nFrom: \n";
				cout << "Label: " << resEdges[i].first[j].from.label << endl;
				for (int k = 0; k < resEdges[i].first[j].from.attName.size(); k++)
				{
					cout << resEdges[i].first[j].from.attName[k] << ": " << resEdges[i].first[j].from.att[k] << "\t";
				}
				cout << endl << endl;;
				cout << "Relation Type: " << resEdges[i].first[j].type << endl;;
				for (int k = 0; k < resEdges[i].first[j].attName.size(); k++)
				{
					cout << resEdges[i].first[j].attName[k] << ": " << resEdges[i].first[j].att[k] << "\t";
				}
				cout << endl;
				cout << "\To: \n";
				cout << "Label: " << resEdges[i].first[j].to.label << endl;
				for (int k = 0; k < resEdges[i].first[j].to.attName.size(); k++)
				{
					cout << resEdges[i].first[j].to.attName[k] << ": " << resEdges[i].first[j].to.att[k] << "\t";
				}
				cout << endl;
			}
			cout << endl;
		}
		for (int i = 0; i < resSpecific.size(); i++)
		{
			cout << "\n" << resSpecific[i].second << ": \n";
			cout << resSpecific[i].first << endl;
		}
		for (int i = 0; i < resSpecifics.size(); i++)
		{
			cout << "\n" << resSpecifics[i].second << ": \n";
			for (int j = 0; j < resSpecifics[i].first.size(); j++)
			{
				cout << resSpecifics[i].first[j] << endl;
			}
		}
	}

};
