/* 046267 Computer Architecture - HW #3 */
/* Implementation (skeleton)  for the dataflow statistics calculator */

#include "dflow_calc.h"
#include <vector>


class Node {
    int key;
    int dst; // every dst is unique because we don't have WAW.
    int latency; // weight - the time for command exec.

    std::vector<Node*> offsprings;
    int num_offsprings;

    public:
        /**
         * @fn Node
         * @brief define single node with two offsprings
         * @param[in] idx the number of line in program.
         * @param[in] dst the writing dest of the command - the key of the node.
         * @param[in] latency the latency of the command - the weight in graph.
         * @param[in] src1 a pointer for the 1st offspring in graph.
         * @param[in] src2 a pointer for the 2nd offspring in graph.
         */
        Node(int idx, int dst, int latency, Node* src1, Node* src2) {
            this->key = idx;
            this->dst = dst;
            this->latency = latency;

            this->offsprings.push_back(src1);
            this->offsprings.push_back(src2);

            this->num_offsprings = 2;
        }

        /**
         * @fn Node
         * @brief define single node without offsprings
         * @param[in] dst - the writing dest of the command - the key of the node.
         * @param[in] latency - the latency of the command - the weight in graph.
         */
        Node(int idx, int dst, int latency) {
            this->key = idx;
            this->dst = dst;
            this->latency = latency;

            this->offsprings.clear();
            this->num_offsprings = 0;
        }

        /**
         * @fn add_offspring
         * @brief adds an offspring to vector
         * @param[in] offspring - a pointer for the offspring.
         */
        void add_offspring(Node* offspring) {
            this->offsprings.push_back(offspring);
            this->num_offsprings++;
        }

        /**
         * @fn get_offspring_at_idx
         * @brief gets the offspring from the vector at given idx.
         * @param[in] idx the desired idx.
         * @return the pointer to the desired offspring.
         */
        Node* get_offspring_at_idx(int idx) {
            return this->offsprings.at(idx);
        }

        /**
         * @fn get_key
         * @brief retruns the key - key of node.
         * @return the key.
         */
        int get_key() {
            return this->key;
        }

        /**
         * @fn get_dst
         * @brief retruns the dst.
         * @return  dst.
         */
        int get_dst() {
            return this->dst;
        }

        /**
         * @fn get_latency
         * @brief retruns the latency - weight of node.
         * @return the weight - latency.
         */
        int get_latency() {
            return this->latency;
        }

        /**
         * @fn get_num_offsprings
         * @brief retruns the number of offsprings - vector's length.
         * @return  number of offsprings.
         */
        int get_num_offsprings() {
            return this->num_offsprings;
        }

        /**
         * @fn find_idx_of_offspring_by_dst
         * @brief finds the index of the offspring from dst of node.
         * @param[in] dst the dst of the desired node.
         * @return index of node
         */
        int find_idx_of_offspring_by_dst(int dst) {
            for (int i = 0; i < this->num_offsprings; i++) {
                if (this->offsprings.at(i)->get_dst() == dst) {
                    return i;
                }
            }

            return -1;
        }

        /**
         * @fn find_idx_of_offspring_by_key
         * @brief finds the index of the offspring from key of node.
         * @param[in] key the key of the desired node.
         * @return index of node
         */
        int find_idx_of_offspring_by_key(int key) {
            for (int i = 0; i < this->num_offsprings; i++) {
                if (this->offsprings.at(i)->get_key() == key) {
                    return i;
                }
            }

            return -1;
        }

        /**
         * @fn remove_offspring_from_dep
         * @brief removes the node with key from offsprings.
         * @param[in] key the key of the desired node.
         * @return 0 on success, 1 if not found.
         */
        int remove_offspring_from_dep(int dst) {
            int idx = this->find_idx_of_offspring_by_dst(dst);
            if (idx == -1) {
                return 1; // nothing to remove;
            }

            this->offsprings.erase(this->offsprings.begin() + idx);
            this->num_offsprings--;
            return 0;
        }

};


// returns max
int max(int a, int b) {
    return a > b ? a : b;
}

// finds idx of desired node inside a vector.
int find_in_vec(std::vector<Node*>& vec, Node* node) {
    for (size_t i = 0; i < vec.size(); i++) {
        if (vec.at(i)->get_key() == node->get_key()) {
            return i;
        }
    }

    return -1;
}

/**
 * @fn search_graph_by_dst
 * @brief searches for node in graph.
 * @param[in] node the "head" of graph.
 * @param[in] dst desired dst.
 * @return the pointer for found node.
 */
Node* search_graph_by_dst(Node* node, int dst) {
    if (!node) {
        return nullptr;
    }

    if (node->get_dst() == dst) {
        return node;
    }

    // checks each branch
    for (int i = 0; i < node->get_num_offsprings(); i++) {
        Node* offspring = search_graph_by_dst(node->get_offspring_at_idx(i), dst);

        // if found - return. uniqueness.
        if(offspring) {
            return offspring;
        }
    }

    return nullptr;
}

/**
 * @fn search_graph_by_key
 * @brief searches for node in graph.
 * @param[in] node the "head" of graph.
 * @param[in] key desired key.
 * @return the pointer for found node.
 */
Node* search_graph_by_key(Node* node, int key) {
    if (!node) {
        return nullptr;
    }

    if (node->get_key() == key) {
        return node;
    }

    // checks each branch
    for (int i = 0; i < node->get_num_offsprings(); i++) {
        Node* offspring = search_graph_by_key(node->get_offspring_at_idx(i), key);

        // if found - return. uniqueness.
        if(offspring) {
            return offspring;
        }
    }

    return nullptr;
}

/**
 * @fn top_sort
 * @brief topological sort for DAG.
 * @param[in] node the starting node of the graph.
 * @param[out] res a reference to a vector that will be appended with sorted nodes.
 */
void top_sort(Node* node, std::vector<Node*>& res) {
    for (int i = 0; i < node->get_num_offsprings(); i++) {
        top_sort(node->get_offspring_at_idx(i), res);
    }

    res.insert(res.begin(), node);
}

ProgCtx analyzeProg(const unsigned int opsLatency[], const InstInfo progTrace[], unsigned int numOfInsts) {
    Node* entry = new Node(-1, -1, 0); // dst and key of entry is -1
    Node* exit = new Node(numOfInsts + 1, -2, 0);  // dst and key of exit is last+1

    exit->add_offspring(entry);

    for (size_t i = 0; i < numOfInsts; i++) {
        // a node can have max 2 srcs
        Node* src1_node = search_graph_by_dst(exit, progTrace[i].src1Idx);    
        Node* src2_node = search_graph_by_dst(exit, progTrace[i].src2Idx); 

        // if not exist then offspring dst=-1
        if (!src1_node) {
            src1_node = entry;
        }

        if (!src2_node) {
            src2_node = entry;
        }
        
        // create new node for new inst
        int dst = progTrace[i].dstIdx;
        unsigned int latency = opsLatency[progTrace[i].opcode];
        Node* node = new Node(i, dst, latency, src1_node, src2_node);

        // remove srcs (adopted offsprings) from exit. ones src is others dst
        exit->remove_offspring_from_dep(progTrace[i].src1Idx);
        exit->remove_offspring_from_dep(progTrace[i].src2Idx);
        
        // adds new command to graph
        exit->add_offspring(node);
    }
    
    return exit;
}

// deletes entire graph excpet entry
void deleteGraph(Node* head) {
    for (int i = 0; i < head->get_num_offsprings(); i++) {
        if (head->get_offspring_at_idx(i)->get_key() >= 0) {
            freeProgCtx(head->get_offspring_at_idx(i));
        }
    }

    delete head;
}

void freeProgCtx(ProgCtx ctx) {
    Node* ctx_node = reinterpret_cast<Node*>(ctx);
    Node* entry = search_graph_by_key(ctx_node, -1); // entry key is -1

    deleteGraph(ctx_node);

    delete entry;
}

int getInstDepth(ProgCtx ctx, unsigned int theInst) {
    Node* node = reinterpret_cast<Node*>(ctx);

    Node* start = search_graph_by_key(node, theInst);

    if(!start) {
        return -1;
    }

    std::vector<Node*> top_sort_res;

    top_sort(start, top_sort_res);

    std::vector<int> d;

    // resets all total weights
    for(size_t i = 0; i < top_sort_res.size(); i++) {
        d.push_back(0);
    }

    // weight maxxing
    for(size_t i = 0; i < d.size(); i++) {
        Node* curr = top_sort_res.at(i);

        for (int j = 0; j < curr->get_num_offsprings(); j++) {

            Node* curr_offspring = curr->get_offspring_at_idx(j);
            int idx_v = find_in_vec(top_sort_res, curr_offspring);
            d.at(idx_v) = max(d.at(idx_v), d.at(i) + curr_offspring->get_latency());
        }
    }

    return d.back();
}

int getInstDeps(ProgCtx ctx, unsigned int theInst, int *src1DepInst, int *src2DepInst) {
    Node* head = reinterpret_cast<Node*>(ctx);

    Node* node = search_graph_by_key(head, theInst);

    if(!node) {
        return -1;
    }

    *src1DepInst = node->get_offspring_at_idx(0)->get_key(); //src1
    *src2DepInst = node->get_offspring_at_idx(1)->get_key(); //src2
    
    return 0;
}

int getProgDepth(ProgCtx ctx) {
    Node* node = reinterpret_cast<Node*>(ctx);
    return getInstDepth(ctx, node->get_key());
}


