/*
Беатрикс Киддо в молодости была наемницей и любила составлять графы связей между её целями.
Она цепляла на стену иголки и связывала их нитками.
Но так как она наполовину японка, ей особенно нравилось, когда цели можно было расположить так, чтобы нитки не пересекались.
Вам известна схема, которую хочет начертить Беатрикс. Выясните, может ли в этот раз схема ей понравиться?
v:количество целей Беатрикс(макс. 1000), n:количество связей между целями(макс. 3000), n пар связей.
Нумерация ведётся с 0. Петли допустимы.
*/

#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <map>
using namespace std;

class graph {
public:
    explicit graph(size_t V) {
        adj.resize(V); // V - number of vertices in graph
    }

    void add(size_t from, size_t to) {
        adj[from].insert(to);
        adj[to].insert(from);
    }

    bool planar() {
        deleteBridges(); // deleting the bridges for gamma algorithm
        auto components = getComponents(); // getting components of connectivity
        for (const auto& component : components) {
            graph tmp(component.size()); // graph for each component
            map<size_t, size_t> transform; // transformation of separated vertices of one component in original graph to graph without useless vertices
            for (size_t i = 0; i < component.size(); ++i) transform[component[i]] = i; // rule of transformation
            for (auto from : component)
                for (auto to : adj[from])
                    tmp.add(transform[from], transform[to]);
            if (!tmp.gamma()) return false; // checking gamma algorithm's answer on component
        }
        return true; // all the components are planar
    }

private:
    vector<set<size_t>> adj;

    vector<vector<size_t>> getComponents() {
        vector<char> colors(adj.size(), 'W');
        vector<vector<size_t>> components;
        for (size_t i = 0; i < adj.size(); ++i)
            if (!adj[i].empty() && colors[i] == 'W') {
                vector<size_t> queue;
                componentsDFS(i, colors, queue);
                components.push_back(queue);
            }
        return components;
    }

    void componentsDFS(size_t point, vector<char>& colors, vector<size_t>& queue) {
        if (colors[point] == 'W') { // use dfs only if point is White
            colors[point] = 'G'; // now we got into the point so it Grey now
            for (auto vertex : adj[point]) componentsDFS(vertex, colors, queue); // dfsing all the kids
            colors[point] = 'B'; // we got out of the point so it is Black now
            queue.push_back(point); // keep point as visited
        }
    }

    void deleteBridges() {
        size_t timer = 0;
        vector<char> colors(adj.size(), 'W');
        vector<size_t> in(adj.size()), low(adj.size());
        vector<pair<size_t, size_t>> bridges;
        for (int i = 0; i < adj.size(); ++i) bridgesDFS(i, -1, timer, colors, in, low, bridges);
        for (auto bridge : bridges) {
            adj[bridge.first].erase(bridge.second);
            adj[bridge.second].erase(bridge.first);
        }
    }

    void bridgesDFS(size_t point, int parent, size_t& timer, vector<char>& colors, vector<size_t>& in, vector<size_t>& low, vector<pair<size_t, size_t>>& bridges) {
        if (colors[point] == 'W') {
            colors[point] = 'B';
            in[point] = low[point] = timer++;
            for (auto to : adj[point])
                if (to != parent) {
                    if (colors[to] == 'B') low[point] = min(low[point], in[to]);
                    else {
                        bridgesDFS(to, point, timer, colors, in, low, bridges);
                        low[point] = min(low[point], low[to]);
                        if (low[to] > in[point]) bridges.push_back(make_pair(point, to));
                    }
                }
        }
    }

    bool gamma() {
        if (adj.size() <= 3) return true; // geometric interpretation

        auto cycle = getSimpleCycle(); // getting any simple cycle in the graph
        if (cycle.empty()) return true; // it's a tree

        vector<size_t> infinityFace = cycle;
        vector<vector<size_t>> faces;
        vector<bool> processedVertices(adj.size(), false);
        vector<set<size_t>> processedEdges(adj.size());

        for (auto vertex : cycle) processedVertices[vertex] = true; // matching these vertices processed
        embedChain(processedEdges, cycle, true); // matching these edges processed
        faces.push_back(infinityFace); // adding infinity face
        faces.push_back(cycle); // adding basement of planar vision

        while (true) { // step of gamma algorithm
            vector<graph> segments = getSegments(processedVertices, processedEdges); // getting all segments we can get now
            if (segments.empty()) return true; // we've already built planar vision of graph
            vector<vector<size_t>> destinationFaces(segments.size()); // vector of faces in which we can put sigment number i
            auto countFaces = amountFacesIncludedSegments(faces, infinityFace, segments, processedVertices, destinationFaces);
            size_t chosenSegment = 0; // number of segment with least number of possible faces to put in
            for (size_t i = 0; i < countFaces.size(); ++i) // searching for a minimum
                if (countFaces[i] < countFaces[chosenSegment]) chosenSegment = i;
            if (countFaces[chosenSegment] == 0) return false; // we already can't put this segment anywhere
            vector<size_t> chain = segments[chosenSegment].getChain(processedVertices); // getting any chain from chosen segment
            for (auto vertex : chain) processedVertices[vertex] = true; // matching these vertices processed
            embedChain(processedEdges, chain, false); // matching these edges processed
            vector<size_t> splittingFace = destinationFaces[chosenSegment]; // we will split this face
            size_t contactFirst = 0, contactSecond = 0; // numbers of contact vertices of chain in splitting face
            for (size_t i = 0; i < splittingFace.size(); ++i) {
                if (splittingFace[i] == chain[0]) contactFirst = i;
                else if (splittingFace[i] == chain[chain.size() - 1]) contactSecond = i;
            }
            vector<size_t> reverseChain = chain;
            if (contactFirst > contactSecond) { // giving order
                swap(contactFirst, contactSecond);
                reverse(chain.begin(), chain.end());
            }
            else reverse(reverseChain.begin(), reverseChain.end());
            vector<size_t> halfFace1, halfFace2;
            for (auto vertex : reverseChain) halfFace1.push_back(vertex);
            for (size_t i = (contactFirst + 1) % splittingFace.size(); i != contactSecond; i = (i + 1) % splittingFace.size())
                halfFace1.push_back(splittingFace[i]);
            if (splittingFace == infinityFace) {
                for (auto vertex : reverseChain) halfFace2.push_back(vertex);
                for (size_t i = (contactFirst - 1 + splittingFace.size()) % splittingFace.size(); i != contactSecond; i = (i - 1 + splittingFace.size()) % splittingFace.size())
                    halfFace2.push_back(splittingFace[i]);
                infinityFace = halfFace1; // refreshing infinity face
            }
            else {
                for (auto vertex : chain) halfFace2.push_back(vertex);
                for (size_t i = (contactSecond + 1) % splittingFace.size(); i != contactFirst; i = (i + 1) % splittingFace.size())
                    halfFace2.push_back(splittingFace[i]);
            }
            faces.erase(find(faces.begin(), faces.end(), splittingFace)); // deleting splited face
            faces.push_back(halfFace1); // adding new face
            faces.push_back(halfFace2); // adding new face
        }
    }

    vector<size_t> getSimpleCycle() {
        vector<char> colors(adj.size(), 'W');
        vector<size_t> cycle;
        bool foundCycle = false, finishCycle = false;
        cyclicDFS(0, -1, colors, cycle, foundCycle, finishCycle);
        return cycle;
    }

    void cyclicDFS(size_t point, int parent, vector<char>& colors, vector<size_t>& cycle, bool& foundCycle, bool& finishCycle) {
        if (!foundCycle && colors[point] == 'W') { // use this dfs only if point is White and cycle didn't find
            colors[point] = 'G'; // now we got into the point so it Grey now
            for (auto vertex : adj[point])
                if (vertex != parent && !foundCycle) { // don't go back to parent and don't waste time
                    if (colors[vertex] == 'G') { // cycle found
                        cycle.push_back(vertex);
                        foundCycle = true;
                    }
                    else cyclicDFS(vertex, point, colors, cycle, foundCycle, finishCycle); // dfsing all the kids
                }
            if (foundCycle) {
                if (point == cycle[0]) finishCycle = true; // cycle finished as snake ate her tail
                if (!finishCycle) cycle.push_back(point); // keep point as visited
            }
            colors[point] = 'B'; // we got out of the point so it is Black now
        }
    }

    vector<size_t> getChain(const vector<bool>& processedVertices) {
        for (size_t vertex = 0; vertex < adj.size(); ++vertex)
            if (processedVertices[vertex] && !adj[vertex].empty()) { // searching for a contact vertex
                vector<char> colors(adj.size(), 'W');
                vector<size_t> chain;
                bool foundChain = false;
                chainDFS(vertex, colors, chain, processedVertices, foundChain);
                return chain;
            }
    }

    void chainDFS(size_t point, vector<char>& colors, vector<size_t>& chain, const vector<bool>& processedVertices, bool& foundChain, int parent = -1) {
        if (!foundChain && colors[point] == 'W') { // use this dfs only if point is White and chain didn't find
            colors[point] = 'G'; // now we got into the point so it Grey now
            for (auto vertex : adj[point])
                if (vertex != parent && !foundChain) { // don't go back to parent and don't waste time
                    if (processedVertices[vertex] && colors[vertex] != 'G') { // second contact vertice found
                        chain.push_back(vertex);
                        foundChain = true;
                    }
                    else chainDFS(vertex, colors, chain, processedVertices, foundChain, point); // dfsing all the kids
                }
            if (foundChain) chain.push_back(point); // keep point as visited
            colors[point] = 'B'; // we got out of the point so it is Black now
        }
    }

    void embedChain(vector<set<size_t>>& processedEdges, const vector<size_t>& chain, bool cyclic) {
        for (size_t i = 0; i < chain.size() - 1; ++i) { // mathching all the edges accept extreme one
            processedEdges[chain[i]].insert(chain[i + 1]);
            processedEdges[chain[i + 1]].insert(chain[i]);
        }
        if (cyclic) { // matching extreme edge
            processedEdges[chain[0]].insert(chain[chain.size() - 1]);
            processedEdges[chain[chain.size() - 1]].insert(chain[0]);
        }
    }

    vector<graph> getSegments(const vector<bool>& processedVertices, const vector<set<size_t>>& processedEdges) {
        vector<graph> segments;
        for (size_t i = 0; i < adj.size(); ++i) // searching for segments with only one edge
            for (auto j : adj[i]) {
                if (find(processedEdges[i].begin(), processedEdges[i].end(), j) == processedEdges[i].end() && processedVertices[i] && processedVertices[j]) {
                    graph tmp(adj.size());
                    tmp.add(i, j);
                    segments.push_back(tmp);
                }
            }
        graph differenceGraph(adj.size()); // difference graph is original graph without processed vertices
        vector<size_t> selfVertices; // to check vertices without edges not with processed vertices
        for (size_t from = 0; from < adj.size(); ++from)
            if (!processedVertices[from]) { // this vertex should be in difference graph
                for (auto to : adj[from])
                    if (!processedVertices[to]) // only edges with vertices in difference graph
                        differenceGraph.add(from, to);
                if (differenceGraph.adj[from].empty()) selfVertices.push_back(from); // it doesn't have edges with vertices in difference graph
            }
        for (const auto& component : differenceGraph.getComponents()) {
            graph tmp(adj.size());
            for (auto from : component)
                for (auto to : adj[from]) tmp.add(from, to); // adding all the edges of vertices in component
            segments.push_back(tmp);
        }
        for (auto vertex : selfVertices) {
            graph tmp(adj.size());
            for (auto to : adj[vertex]) tmp.add(vertex, to); // adding all the edges (edges with processed vertices actually) of self vertices
            segments.push_back(tmp);
        }
        return segments;
    }

    vector<size_t> amountFacesIncludedSegments(const vector<vector<size_t>>& faces, const vector<size_t>& infinityFace, vector<graph> segments, const vector<bool>& processedVertices, vector<vector<size_t>>& destinationFaces) {
        vector<size_t> count(segments.size(), 0);
        for (size_t i = 0; i < segments.size(); ++i) // each segment
            for (const auto& face : faces) // to each face
                if (faceContainsSegment(face, segments[i], processedVertices)) {
                    destinationFaces[i] = face; // setting destination face as we can put segment there
                    ++count[i]; // one more good face
                }
        return count;
    }

    bool faceContainsSegment(const vector<size_t>& face, const graph& segment, const vector<bool>& processedVertices) {
        for (size_t i = 0; i < adj.size(); ++i)
            for (auto j : segment.adj[i])
                if ((processedVertices[i] && find(face.begin(), face.end(), i) == face.end()) || (processedVertices[j] && find(face.begin(), face.end(), j) == face.end()))
                    return false; // if exists contact vertex not belonging to this face
        return true;
    }
};

int main() {
    size_t V, E;
    cin >> V >> E;
    graph G(V);
    for (size_t i = 0; i < E; ++i) {
        size_t from, to;
        cin >> from >> to;
        G.add(from, to);
    }
    if (G.planar()) cout << "YES";
    else cout << "NO";
    return 0;
}