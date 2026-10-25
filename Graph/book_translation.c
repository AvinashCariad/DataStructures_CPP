#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LANGS 101
#define WORD_LENGTH 16

typedef struct edge {
  int to_lang, cost;
  struct edge *next;
} edge;

typedef int board[MAX_LANGS];
typedef int positions[MAX_LANGS];

void add_position(int from_lang, int to_lang,
                  positions new_positions, int *num_new_positions,
                  board min_moves) {
  if (min_moves[to_lang] == -1) {
    min_moves[to_lang] = 1 + min_moves[from_lang];
    new_positions[*num_new_positions] = to_lang;
    (*num_new_positions)++;
  }
}

void find_distances(edge *adj_list[], int num_langs, board min_costs) {
  static board min_moves;
  static positions cur_positions, new_positions;
  int num_cur_positions, num_new_positions;
  int i, from_lang, added_lang, best;
  edge *e;
  for (i = 0; i < num_langs; i++) {
    min_moves[i] = -1;
    min_costs[i] = -1;
  }
  min_moves[0] = 0;
  cur_positions[0] = 0;
  num_cur_positions = 1;

  while (num_cur_positions > 0) {
    num_new_positions = 0;
    for (i = 0; i < num_cur_positions; i++) {
      from_lang = cur_positions[i];
      e = adj_list[from_lang];

      while (e) {
        add_position(from_lang, e->to_lang,
                     new_positions, &num_new_positions, min_moves);
        e = e->next;
      }
    }

    for (i = 0; i < num_new_positions; i++) {
      added_lang = new_positions[i];
      e = adj_list[added_lang];
      best = -1;
      while (e) {
        if (min_moves[e->to_lang] + 1 == min_moves[added_lang] &&
            (best == -1 || e->cost < best))
          best = e->cost;
        e = e->next;
      }
      min_costs[added_lang] = best;
    }

    num_cur_positions = num_new_positions;
    for (i = 0; i < num_cur_positions; i++)
      cur_positions[i] = new_positions[i];
  }
}

void solve(int num_langs, board min_costs) {
  int i, total = 0;
  for (i = 1; i < num_langs; i++)
    if (min_costs[i] == -1) {
      printf("Impossible\n");
      return;
    } else {
      total = total + min_costs[i];
  }
  printf("%d\n", total);
}

/*based on https://stackoverflow.com/questions/16870485 */
char *read_word(int size) {
  char *str;
  int ch;
  int len = 0;
  str = malloc(size);
  if (str == NULL) {
    fprintf(stderr, "malloc error\n");
    exit(1);
  }
  while ((ch = getchar()) != EOF && (ch != ' ') && (ch != '\n')) {
    str[len++] = ch;
    if (len == size) {
      size = size * 2;
      str = realloc(str, size);
      if (str == NULL) {
        fprintf(stderr, "realloc error\n");
        exit(1);
      }
    }
  }
  str[len] = '\0';
  return str;
}

int find_lang(char *langs[], char *lang) {
  int i = 0;
    while (strcmp(langs[i], lang) != 0)
      i++;
  return i;
}



// main code harness:
int main(void) {
  static edge *adj_list[MAX_LANGS] = {NULL};
  static char *lang_names[MAX_LANGS];
  int i, num_targets, num_translators, cost, from_index, to_index;
  char *from_lang, *to_lang;
  edge *e;
  static board min_costs;
  scanf("%d%d\n", &num_targets, &num_translators);
  lang_names[0] = "English";

  for (i = 1; i <= num_targets; i++)
    lang_names[i] = read_word(WORD_LENGTH);

  for (i = 0; i < num_translators; i++) {
    from_lang = read_word(WORD_LENGTH);
    to_lang = read_word(WORD_LENGTH);
    scanf("%d\n", &cost);
    from_index = find_lang(lang_names, from_lang);
    to_index = find_lang(lang_names, to_lang);
    e = malloc(sizeof(edge));
    if (e == NULL) {
      fprintf(stderr, "malloc error\n");
      exit(1);
    }
    e->to_lang = to_index;
    e->cost = cost;
    e->next = adj_list[from_index];
    adj_list[from_index] = e;
    e = malloc(sizeof(edge));
    if (e == NULL) {
      fprintf(stderr, "malloc error\n");
      exit(1);
    }
    e->to_lang = from_index;
    e->cost = cost;
    e->next = adj_list[to_index];
    adj_list[to_index] = e;
  }
  find_distances(adj_list, num_targets + 1, min_costs);
  solve(num_targets + 1, min_costs);
  return 0;
}


/*

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <queue>
#include <algorithm>

using namespace std;

const int INF = 1e9;

// Structure to represent a translation option (edge)
struct Edge {
    int to;
    int cost;
};

int main() {
    // Optimize standard I/O operations for performance
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    // Map language names to integer IDs
    unordered_map<string, int> lang_id;
    lang_id["English"] = 0;

    vector<string> targets(n);
    for (int i = 0; i < n; ++i) {
        cin >> targets[i];
        lang_id[targets[i]] = i + 1;
    }

    // Total unique languages = n target languages + English (index 0)
    int total_langs = n + 1;
    vector<vector<Edge>> adj(total_langs);

    for (int i = 0; i < m; ++i) {
        string u_str, v_str;
        int cost;
        cin >> u_str >> v_str >> cost;

        // Only process translation edges if both languages exist in our pool
        if (lang_id.count(u_str) && lang_id.count(v_str)) {
            int u = lang_id[u_str];
            int v = lang_id[v_str];
            adj[u].push_back({v, cost});
            adj[v].push_back({u, cost});
        }
    }

    // BFS tracking arrays
    vector<int> dist(total_langs, INF);
    vector<int> min_cost(total_langs, INF);
    queue<int> q;

    // Initialize English (source node)
    dist[0] = 0;
    min_cost[0] = 0;
    q.push(0);

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (const auto& edge : adj[u]) {
            int v = edge.to;
            int cost = edge.cost;

            // Scenario 1: Found a shorter path (fewer translation steps) to language v
            if (dist[u] + 1 < dist[v]) {
                dist[v] = dist[u] + 1;
                min_cost[v] = cost;
                q.push(v);
            } 
            // Scenario 2: Found an alternative path with the same number of translation steps
            else if (dist[u] + 1 == dist[v]) {
                min_cost[v] = min(min_cost[v], cost);
            }
        }
    }

    // Calculate final outcome
    long long total_min_cost = 0;
    bool possible = true;

    for (int i = 1; i < total_langs; ++i) {
        if (dist[i] == INF) {
            possible = false;
            break;
        }
        total_min_cost += min_cost[i];
    }

    if (possible) {
        cout << total_min_cost << "\n";
    } else {
        cout << "Impossible\n";
    }

    return 0;
}



















*/