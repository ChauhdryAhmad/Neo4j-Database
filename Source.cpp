#include<iostream>
#include"Neo4j.h"



using namespace std;


int main()
{

    Vector<string>q;
        q.push_back("CREATE (Dhawan:player {name: \"Shikar Dhawan\", YOB: 1985, POB: \"Delhi\"})");
        q.push_back("CREATE (Ind:Country {name: \"India\"})");
        q.push_back("CREATE (Pak:Country {name: \"Pakistan\"})");
        q.push_back("CREATE (B:player {name: \"Babar Azam\", YOB: 1985, POB: \"Lahore\"})");
        q.push_back("CREATE (A:player {name: \"Virat Kohli\", YOB: 1985, POB: \"Islamabad\"})");
        q.push_back("CREATE (S:player {name: \"Messi\", YOB: 1985, POB: \"Lahore\"})");
        q.push_back("CREATE (B)-[r:BATSMAN_OF]->(Pak)");
        q.push_back("CREATE (A)-[a:BATSMAN_OF]->(Pak)");
        q.push_back("CREATE (S)-[b:BATSMAN_OF]->(Pak)");

        q.push_back("CREATE (Dhawan)-[c:BATSMAN_OF]->(Ind)");

        graph g;
        g.processor(q);
        cout << "Node created\n\n\n";

        Vector<string>s;
        s.push_back("MERGE (a:player {name:\"Messi\"})");
        s.push_back("ON CREATE SET a.YOB = 2004");
        s.push_back("ON MATCH SET a.YOB = 2024");
        s.push_back("RETURN a");

        g.processor(s);

        s.pop_back();
        s.pop_back();
        s.pop_back();
        s.pop_back();

        s.push_back("MATCH (n)");
        s.push_back("RETURN n");
        g.processor(s);

        /*s.push_back("SAVE (save.txt)");
        g.processor(s);*/

        /*graph g;

        Vector<string>q;
        q.push_back("READ (temp.txt, people)");
        g.processor(q);
        q.pop_back();
        q.push_back("MATCH (n)");
        q.push_back("RETURN n");
        g.processor(q);*/
}