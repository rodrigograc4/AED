//
// AED, November 2022 (Tomás Oliveira e Silva)
//
// Second practical assignement (word ladder)
//
// Place your student numbers and names here
//   N.Mec. 107637  Name: André Oliveira
//   N.Mec. 107634  Name: Duarte Cruz
//   N.Mec. 107359  Name: Rodrigo Graça
//
// Do as much as you can
//   1) MANDATORY: complete the hash table code
//      *) hash_table_create
//      *) hash_table_grow
//      *) hash_table_free
//      *) find_word
//      +) add code to get some statistical data about the hash table
//   2) HIGHLY RECOMMENDED: build the graph (including union-find data) -- use the similar_words function...
//      *) find_representative
//      *) add_edge
//   3) RECOMMENDED: implement breadth-first search in the graph
//      *) breadh_first_search
//   4) RECOMMENDED: list all words belonginh to a connected component
//      *) breadh_first_search
//      *) list_connected_component
//   5) RECOMMENDED: find the shortest path between to words
//      *) breadh_first_search
//      *) path_finder
//      *) test the smallest path from bem to mal
//         [ 0] bem
//         [ 1] tem
//         [ 2] teu
//         [ 3] meu
//         [ 4] mau
//         [ 5] mal
//      *) find other interesting word ladders
//   6) OPTIONAL: compute the diameter of a connected component and list the longest word chain
//      *) breadh_first_search
//      *) connected_component_diameter
//   7) OPTIONAL: print some statistics about the graph
//      *) graph_info
//   8) OPTIONAL: test for memory leaks
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//
// static configuration
//

#define _max_word_size_ 32

//
// data structures (SUGGESTION --- you may do it in a different way)
//

typedef struct adjacency_node_s adjacency_node_t;
typedef struct hash_table_node_s hash_table_node_t;
typedef struct hash_table_s hash_table_t;

struct adjacency_node_s
{
  adjacency_node_t *next;    // link to th enext adjacency list node
  hash_table_node_t *vertex; // the other vertex
};

struct hash_table_node_s
{
  // the hash table data
  char word[_max_word_size_]; // the word
  hash_table_node_t *next;    // next hash table linked list node
  // the vertex data
  adjacency_node_t *head;      // head of the linked list of adjancency edges
  int visited;                 // visited status (while not in use, keep it at 0)
  hash_table_node_t *previous; // breadth-first search parent
  // the union find data
  hash_table_node_t *representative; // the representative of the connected component this vertex belongs to
  int number_of_vertices;            // number of vertices of the conected component (only correct for the representative of each connected component)
  int number_of_edges;               // number of edges of the conected component (only correct for the representative of each connected component)
};

struct hash_table_s
{
  unsigned int hash_table_size;   // the size of the hash table array
  unsigned int number_of_entries; // the number of entries in the hash table
  unsigned int number_of_edges;   // number of edges (for information purposes only)
  hash_table_node_t **heads;      // the heads of the linked lists
  float average_list_length;      // the average length of the linked lists
  unsigned int max_list_length;   // the maximum length of a linked list
  unsigned int min_list_length;   // the minimum length of a linked list
  unsigned int empty_lists;       // the number of empty linked lists
};

//
// allocation and deallocation of linked list nodes (done)
//

static adjacency_node_t *allocate_adjacency_node(void)
{
  adjacency_node_t *node;

  node = (adjacency_node_t *)malloc(sizeof(adjacency_node_t));
  if (node == NULL)
  {
    fprintf(stderr, "allocate_adjacency_node: out of memory\n");
    exit(1);
  }
  return node;
}

static hash_table_node_t *allocate_hash_table_node(void)
{
  hash_table_node_t *node;

  node = (hash_table_node_t *)malloc(sizeof(hash_table_node_t));
  if (node == NULL)
  {
    fprintf(stderr, "allocate_hash_table_node: out of memory\n");
    exit(1);
  }
  return node;
}

//
// hash table stuff (mostly to be done)
//

unsigned int crc32(const char *str)
{
  static unsigned int table[256];
  unsigned int crc;

  if (table[1] == 0u) // do we need to initialize the table[] array?
  {
    unsigned int i, j;

    for (i = 0u; i < 256u; i++)
      for (table[i] = i, j = 0u; j < 8u; j++)
        if (table[i] & 1u)
          table[i] = (table[i] >> 1) ^ 0xAED00022u; // "magic" constant
        else
          table[i] >>= 1;
  }
  crc = 0xAED02022u; // initial value (chosen arbitrarily)
  while (*str != '\0')
    crc = (crc >> 8) ^ table[crc & 0xFFu] ^ ((unsigned int)*str++ << 24);
  return crc;
}

static hash_table_t *hash_table_create(void)
{
  // create a new hash table
  hash_table_t *hash_table;

  // allocate the hash table
  hash_table = (hash_table_t *)malloc(sizeof(hash_table_t));

  // check for allocation errors
  if (hash_table == NULL)
  {
    fprintf(stderr, "create_hash_table: out of memory\n");
    return -1;
  }

  // initialize the hash table
  hash_table->hash_table_size = 250;
  hash_table->number_of_entries = 0;
  hash_table->number_of_edges = 0;

  // allocate the array of linked list heads
  hash_table->heads = (hash_table_node_t **)malloc(hash_table->hash_table_size * sizeof(hash_table_node_t *));

  // check for allocation errors
  if (hash_table->heads == NULL)
  {
    fprintf(stderr, "create_hash_table: out of memory\n");
    return -1;
  }

  // Fill the array of linked list heads with NULL
  for (int i = 0; i < hash_table->hash_table_size; i++)
  {
    hash_table->heads[i] = NULL;
  }

  return hash_table;
}

static void hash_table_grow(hash_table_t *hash_table)
{
  // the old and new array of linked list heads
  hash_table_node_t **old_heads, **new_heads, *node, *next;
  unsigned int old_size, i;

  // save a pointer to the old array of linked list heads and its size
  old_heads = hash_table->heads;
  old_size = hash_table->hash_table_size;

  // create a new hash table with a larger size
  hash_table->hash_table_size *= 2;
  new_heads = (hash_table_node_t **)malloc(hash_table->hash_table_size * sizeof(hash_table_node_t *));

  // Fill the array of linked list heads with NULL
  for (i = 0; i < hash_table->hash_table_size; i++)
    new_heads[i] = NULL;

  if (new_heads == NULL)
  {
    fprintf(stderr, "Error: out of memory");
    exit(1);
  }

  // rehash the entries from the old array to the new one
  for (i = 0; i < old_size; i++)
  {
    node = old_heads[i];
    while (node != NULL)
    {

      next = node->next;

      size_t value = crc32(node->word) % hash_table->hash_table_size;
      node->next = new_heads[value];
      new_heads[value] = node;

      node = next;
    }
  }
  free(old_heads);
  hash_table->heads = new_heads;
}

static void hash_table_free(hash_table_t *hash_table)
{
  hash_table_node_t *node;
  hash_table_node_t *temp;
  adjacency_node_t *adj_node;
  adjacency_node_t *temp_adj;
  unsigned int i;

  for (i = 0; i < hash_table->hash_table_size; i++)
  {
    node = hash_table->heads[i];
    while (node != NULL)
    {
      temp = node;
      adj_node = node->head;
      while (adj_node != NULL)
      {
        temp_adj = adj_node;
        adj_node = adj_node->next;
        free(temp_adj);
      }
      node = node->next;
      free(temp);
    }
  }

  // Free the memory used by the array of linked list heads
  free(hash_table->heads);

  // Free the memory used by the hash table
  free(hash_table);
}

static hash_table_node_t *find_word(hash_table_t *hash_table, const char *word, int insert_if_not_found)
{
  hash_table_node_t *node;
  unsigned int i;

  // find the word in the hash table
  i = crc32(word) % hash_table->hash_table_size;
  node = hash_table->heads[i];
  while (node != NULL)
  {
    if (strcmp(node->word, word) == 0)
      return node;
    node = node->next;
  }

  // if the word is not found, insert it into the hash table
  if (insert_if_not_found && strlen(word) < _max_word_size_)
  {
    node = allocate_hash_table_node();          // allocate a new node
    strncpy(node->word, word, _max_word_size_); // copy the word into the node
    node->representative = node;                // the representative of a node is itself
    node->next = hash_table->heads[i];          // insert the node at the head of the linked list
    node->previous = NULL;
    node->number_of_edges = 0;
    node->number_of_vertices = 1;
    node->visited = 0;
    node->head = NULL;
    hash_table->heads[i] = node;
    hash_table->number_of_entries++;
    // if the number of entries exceeds the size of the hash table, grow the hash table
    if (hash_table->number_of_entries > hash_table->hash_table_size)
      hash_table_grow(hash_table);
    return node;
  }

  return NULL;
}

//
// add edges to the word ladder graph
//

hash_table_node_t *find_representative(hash_table_node_t *node)
{
  hash_table_node_t *representative, *next_node, *actual_node;

  // Find the representative element by following the chain of representatives until we reach an element that points to itself
  for (representative = node; representative != representative->representative; representative = representative->representative)
    ;

  // Do a second pass to apply path compression, which flattens the tree representation of the disjoint-set and improves the efficiency of future find operations
  for (next_node = node; next_node != representative; next_node = next_node->representative)
  {
    actual_node = next_node->representative;
    next_node->representative = representative;
  }

  return representative;
}

static void add_edge(hash_table_t *hash_table, hash_table_node_t *from, const char *word)
{
  hash_table_node_t *to, *from_representative, *to_representative;
  adjacency_node_t *linkfrom, *linkto;

  // Find the word in the hash table
  from_representative = find_representative(from);
  to = find_word(hash_table, word, 0);

  // If the word is not in the hash table, or if the two vertices are the same, return without adding an edge
  if (to == NULL || to == from)
    return;

  // Find the representatives of the two connected components
  to_representative = find_representative(to);

  // If the vertices are already in the same connected component, increment the number of vertices in the component
  if (from_representative == to_representative)
  {
    from_representative->number_of_vertices++;
  }

  // If the vertices are not in the same connected component, merge the two connected components
  if (from_representative != to_representative)
  {
    if (from_representative->number_of_vertices < to_representative->number_of_vertices)
    {
      from_representative->representative = to_representative;
      to_representative->number_of_vertices += from_representative->number_of_vertices;
      to_representative->number_of_edges += from_representative->number_of_edges;
    }
    else
    {
      to_representative->representative = from_representative;
      from_representative->number_of_vertices += to_representative->number_of_vertices;
      from_representative->number_of_edges += to_representative->number_of_edges;
    }
  }

  // Allocate two adjacency nodes to represent the edge between the vertices
  linkfrom = allocate_adjacency_node();
  linkto = allocate_adjacency_node();

  // If allocation fails, print an error message and exit
  if (linkfrom == NULL || linkto == NULL)
  {
    fprintf(stderr, "add_edge: out of memory\n");
    exit(1);
  }

  // Update the linked lists of adjacency nodes for each vertex to include the new nodes
  linkfrom->vertex = to;
  linkfrom->next = from->head;
  from->head = linkfrom;

  linkto->vertex = from;
  linkto->next = to->head;
  to->head = linkto;

  from_representative->number_of_edges++;
  to_representative->number_of_edges++;
  hash_table->number_of_edges++;
  return;
}

//
// generates a list of similar words and calls the function add_edge for each one
//
// man utf8 for details on the uft8 encoding
//

static void break_utf8_string(const char *word, int *individual_characters)
{
  int byte0, byte1;

  while (*word != '\0')
  {
    byte0 = (int)(*(word++)) & 0xFF;
    if (byte0 < 0x80)
      *(individual_characters++) = byte0; // plain ASCII character
    else
    {
      byte1 = (int)(*(word++)) & 0xFF;
      if ((byte0 & 0b11100000) != 0b11000000 || (byte1 & 0b11000000) != 0b10000000)
      {
        fprintf(stderr, "break_utf8_string: unexpected UFT-8 character\n");
        exit(1);
      }
      *(individual_characters++) = ((byte0 & 0b00011111) << 6) | (byte1 & 0b00111111); // utf8 -> unicode
    }
  }
  *individual_characters = 0; // mark the end!
}

static void make_utf8_string(const int *individual_characters, char word[_max_word_size_])
{
  int code;

  while (*individual_characters != 0)
  {
    code = *(individual_characters++);
    if (code < 0x80)
      *(word++) = (char)code;
    else if (code < (1 << 11))
    { // unicode -> utf8
      *(word++) = 0b11000000 | (code >> 6);
      *(word++) = 0b10000000 | (code & 0b00111111);
    }
    else
    {
      fprintf(stderr, "make_utf8_string: unexpected UFT-8 character\n");
      exit(1);
    }
  }
  *word = '\0'; // mark the end
}

static void similar_words(hash_table_t *hash_table, hash_table_node_t *from)
{
  static const int valid_characters[] =
      {                                                                                          // unicode!
       0x2D,                                                                                     // -
       0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D,             // A B C D E F G H I J K L M
       0x4E, 0x4F, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A,             // N O P Q R S T U V W X Y Z
       0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D,             // a b c d e f g h i j k l m
       0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A,             // n o p q r s t u v w x y z
       0xC1, 0xC2, 0xC9, 0xCD, 0xD3, 0xDA,                                                       // Á Â É Í Ó Ú
       0xE0, 0xE1, 0xE2, 0xE3, 0xE7, 0xE8, 0xE9, 0xEA, 0xED, 0xEE, 0xF3, 0xF4, 0xF5, 0xFA, 0xFC, // à á â ã ç è é ê í î ó ô õ ú ü
       0};
  int i, j, k, individual_characters[_max_word_size_];
  char new_word[2 * _max_word_size_];

  break_utf8_string(from->word, individual_characters);
  for (i = 0; individual_characters[i] != 0; i++)
  {
    k = individual_characters[i];
    for (j = 0; valid_characters[j] != 0; j++)
    {
      individual_characters[i] = valid_characters[j];
      make_utf8_string(individual_characters, new_word);
      // avoid duplicate cases
      if (strcmp(new_word, from->word) > 0)
        add_edge(hash_table, from, new_word);
    }
    individual_characters[i] = k;
  }
}

//
// breadth-first search
//
// returns the number of vertices visited; if the last one is goal, following the previous links gives the shortest path between goal and origin
//

static int breadh_first_search(int maximum_number_of_vertices, hash_table_node_t **list_of_vertices, hash_table_node_t *origin, hash_table_node_t *goal)
{
  // Indices for the read and write positions of the queue
  int read = 0, write = 1;
  // Initialize the first position of the queue with the origin vertex [origin, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL]
  list_of_vertices[0] = origin;
  // Initialize the previous vertex of the origin as NULL
  origin->previous = NULL;
  // Mark the origin vertex as visited
  origin->visited = 1;
  int found = 0;
  // While the queue is not empty
  while (read != write)
  {
    // node is set to the head of the origin vertex to begin traversing the linked list
    adjacency_node_t *node = list_of_vertices[read++]->head;
    if (found == 1)
    {
      break;
    }
    while (node != NULL) // Traverse the linked list
    {
      // Check if the vertex has already been visited
      if (node->vertex->visited == 0)
      {
        node->vertex->visited = 1;
        // The previous vertex of this vertex is the vertex before it in the queue
        node->vertex->previous = list_of_vertices[read - 1];
        // Add the vertex to the queue using the write position
        list_of_vertices[write++] = node->vertex;
        if (node->vertex == goal)
        {
          found = 1;
          break;
        }
      }
      node = node->next;
    }
  }
  for (int i = 0; i < write; i++)
  {
    list_of_vertices[i]->visited = 0;
  }
  return write;
}

//
// list all vertices belonging to a connected component
//

static int list_connected_component(hash_table_t *hash_table, const char *word, int option)
{
  // Find the given word in the hash table
  hash_table_node_t *node = find_word(hash_table, word, 0);
  int vertices = 0;
  if (node == NULL)
  {
    fprintf(stderr, "Word not found\n");
    return -1;
  }

  // Get the representative node of the connected component containing the word
  hash_table_node_t *representative = find_representative(node);
  // Allocate memory for a list of vertices in the connected component
  hash_table_node_t **list_of_vertices = (hash_table_node_t **)malloc(representative->number_of_vertices * sizeof(hash_table_node_t *));

  // Perform a breadth-first search to find all the vertices in the connected component
  int num_vertices = breadh_first_search(hash_table->number_of_entries, list_of_vertices, node, NULL);
  // Print all the vertices in the connected component
  if (option == 1)
  {
    printf("Connected component:\n");
  }

  for (int i = 0; i < num_vertices; i++)
  {
    vertices++;
    if (option == 1)
    {
      printf("[%d] %s\n", vertices, list_of_vertices[i]->word);
    }
  }

  free(list_of_vertices);

  return vertices;
}

//
// compute the diameter of a connected component
//

static int largestDiameter, smallestDiameter, diametersSum, numDiameters;
static hash_table_node_t **largestDiameterList;
static int maxNumVertices;

static connected_component_diameter(hash_table_node_t *node)
{
  int diameter = -1;
  int numVertices;
  hash_table_node_t **vertices, **temporaryList;

  maxNumVertices = find_representative(node)->number_of_vertices;
  vertices = (hash_table_node_t **)malloc(maxNumVertices * sizeof(hash_table_node_t *));
  temporaryList = (hash_table_node_t **)malloc(maxNumVertices * sizeof(hash_table_node_t *));

  if (vertices == NULL || temporaryList == NULL)
  {
    fprintf(stderr, "connected_component_diameter: out of memory\n");
    exit(1);
  }

  numVertices = breadh_first_search(maxNumVertices, vertices, node, NULL);

  for (int i = 0; i < numVertices; i++)
  {
    int temporaryNumVertices = breadh_first_search(maxNumVertices, temporaryList, vertices[i], vertices[i]);
    int temporaryDiameter = 0;
    hash_table_node_t *palavras = temporaryList[temporaryNumVertices - 1];

    while (palavras != NULL)
    {
      temporaryDiameter++;
      palavras = palavras->previous;
    }

    if (temporaryDiameter > diameter)
    {
      diameter = temporaryDiameter - 1;
    }
  }

  if (diameter > largestDiameter)
  {
    largestDiameter = diameter;

    largestDiameterList = (hash_table_node_t **)malloc(numVertices * sizeof(hash_table_node_t *));

    if (largestDiameterList == NULL)
    {
      fprintf(stderr, "connected_component_diameter: out of memory\n");
      exit(1);
    }

    largestDiameterList = vertices;
  }

  if (diameter < smallestDiameter)
  {
    smallestDiameter = diameter;
  }

  if (diameter == -1)
  {
    printf("connected_component_diameter: diameter not found\n");
    return -1;
  }

  numDiameters++;
  diametersSum += diameter;

  free(temporaryList);
  free(vertices);
}

//
// find the shortest path from a given word to another given word
//

static void path_finder(hash_table_t *hash_table, const char *from_word, const char *to_word)
{
  // Find the hash table nodes corresponding to the given words.
  hash_table_node_t *from, *fromRepresentative, *to, *toRepresentative;
  from = find_word(hash_table, from_word, 0);
  to = find_word(hash_table, to_word, 0);

  // Find the representatives of the connected components containing the words.
  fromRepresentative = find_representative(from);
  toRepresentative = find_representative(to);

  // If one of the words doesn't exist in the hash table, print an error message and return.
  if (from == NULL || to == NULL)
  {
    printf("One of the words doesn't exist\n");
    return;
  }

  // If the words aren't in the same connected component, print an error message and return.
  if (fromRepresentative != toRepresentative)
  {
    printf("The words aren't in the same connected component\n");
    return;
  }

  // Allocate an array of pointers to hash table nodes to store the vertices of the shortest path.
  hash_table_node_t **vertices = malloc(sizeof(hash_table_node_t *) * fromRepresentative->number_of_vertices);

  // Find the shortest path between the two words using a breadth-first search.
  int path = breadh_first_search(fromRepresentative->number_of_vertices, vertices, to, from);

  // Print the words on the shortest path in order, starting from the destination word and ending at the source word.
  hash_table_node_t *palavras = vertices[path - 1];
  int ordem = 0;
  while (palavras != NULL)
  {
    printf("[%d] %s \n", ordem, palavras->word);
    ordem++;
    palavras = palavras->previous;
  }

  // Free the array of vertices.
  free(vertices);
}

//
// some graph information (optional)
//

static int find_connected_component_representatives(hash_table_t *hash_table, hash_table_node_t **representatives)
{
  int nrRepresentatives = 0;

  // Find the representatives of each connected component
  for (int i = 0; i < hash_table->hash_table_size; i++)
  {
    for (hash_table_node_t *vertex = hash_table->heads[i]; vertex != NULL; vertex = vertex->next)
    {
      // Find the representative of the connected component
      hash_table_node_t *representative = find_representative(vertex);

      // Add the representative to the array if it has not already been added
      if (!representative->visited)
      {
        representatives[nrRepresentatives++] = representative->word;
        representative->visited = 1;
      }
    }
  }

  // Reset the visited status of all vertices
  for (int i = 0; i < hash_table->hash_table_size; i++)
  {
    for (hash_table_node_t *vertex = hash_table->heads[i]; vertex != NULL; vertex = vertex->next)
    {
      vertex->visited = 0;
    }
  }

  return nrRepresentatives;
}

static void graph_info(hash_table_t *hash_table)
{
  // Allocate an array to store representatives (one representative per connected component)
  hash_table_node_t **representatives = malloc(sizeof(hash_table_node_t) * hash_table->hash_table_size);

  // Find the representatives for each connected component in the hash table
  int nrRepresentatives = find_connected_component_representatives(hash_table, representatives);

  // Reset the global variables that track the largest and smallest diameters
  largestDiameter = 0;
  smallestDiameter = hash_table->number_of_entries;

  // For each representative node, find the diameter of its connected component
  for (int i = 0; i < nrRepresentatives; i++)
  {
    connected_component_diameter(representatives[i]);
  }

  // calcula  tamanho maximo de componente conexa
  int min = hash_table->number_of_entries;
  int max = 0;
  int total = 0;
  for (int i = 0; i < nrRepresentatives; i++)
  {
    int size = list_connected_component(hash_table, representatives[i]->word, 2);
    if (size > max)
    {
      max = size;
    }
    if (size < min)
    {
      min = size;
    }

    total += size;
  }

  // Print out various statistics about the graph
  printf("\nNumber of edges: %u\n", hash_table->number_of_edges);
  printf("Number of vertices: %u\n", hash_table->number_of_entries);
  printf("Number of different representatives: %d\n", numDiameters);
  printf("Largest connected component: %u\n", max);
  printf("Smallest connected component: %u\n", min);
  printf("Average connected component: %.2f\n", (float)total / nrRepresentatives);
  printf("Largest diameter: %d\n", largestDiameter);
  printf("Smallest diameter: %d\n", smallestDiameter);
  printf("Average of diameters: %.2f\n\n", (float)diametersSum / numDiameters);

  // Print an example of a word chain in the largest connected component
  printf("Largest diameter example: \n");

  for (int i = 0; i < largestDiameter + 1; i++)
  {
    printf("[%d] %s \n", i, largestDiameterList[i]->word);
  }
  printf("\n");

  free(representatives);
}

static void hash_table_info(hash_table_t *hash_table)
{
  unsigned int total_list_length = 0;
  unsigned int list_length;
  hash_table_node_t *node;
  unsigned int i;

  // Initialize the statistical data
  hash_table->average_list_length = 0.0;
  hash_table->max_list_length = 0;
  hash_table->min_list_length = _max_word_size_;
  hash_table->empty_lists = 0;

  // Iterate through the array of linked list heads and calculate the statistical data
  for (i = 0; i < hash_table->hash_table_size; i++)
  {
    list_length = 0;
    node = hash_table->heads[i];
    while (node != NULL)
    {
      list_length++;
      node = node->next;
    }
    if (list_length > hash_table->max_list_length)
    {
      hash_table->max_list_length = list_length;
    }
    if (list_length < hash_table->min_list_length && list_length != 0)
    {
      hash_table->min_list_length = list_length;
    }
    if (list_length == 0)
    {
      hash_table->empty_lists++;
    }
    total_list_length += list_length;
  }

  // Calculate the average list length
  hash_table->average_list_length = (float)total_list_length / hash_table->hash_table_size;

  // Print the statistical data
  printf("\nNumber of entries: %u\n", hash_table->number_of_entries);
  printf("Hash table size: %u\n", hash_table->hash_table_size);
  printf("Average list length: %.2f\n", hash_table->average_list_length);
  printf("Max list length: %u\n", hash_table->max_list_length);
  printf("Min list length: %u\n", hash_table->min_list_length);
  printf("Number of empty lists: %u\n\n", hash_table->empty_lists);
}

//
// main program
//

int main(int argc, char **argv)
{
  char word[100], from[100], to[100];
  hash_table_t *hash_table;
  hash_table_node_t *node;
  unsigned int i;
  int command;
  FILE *fp;
  printf("Inicialização do programa\n");
  // initialize hash table
  hash_table = hash_table_create();
  // read words
  fp = fopen((argc < 2) ? "wordlist-big-latest.txt" : argv[1], "rb");
  if (fp == NULL)
  {
    fprintf(stderr, "main: unable to open the words file\n");
    exit(1);
  }

  while (fscanf(fp, "%99s", word) == 1)
  {
    (void)find_word(hash_table, word, 1);
    // printf("%d %d\n", hash_table->hash_table_size, hash_table->number_of_entries);
  }
  fclose(fp);
  // return 0;

  //   find all similar words
  for (i = 0u; i < hash_table->hash_table_size; i++)
  {
    for (node = hash_table->heads[i]; node != NULL; node = node->next)
    {
      similar_words(hash_table, node);
    }
  }

  /* for (unsigned int i = 0; i < hash_table->hash_table_size; i++) // loop through the hash table
  {
    hash_table_node_t *node = hash_table->heads[i]; // set node to the first element of the hash table
    while (node != NULL)                            // while the node has a next node
    {
      hash_table_node_t *temp = node;          // set temp to the node
      node = node->next;                       // set node to the next node                    // free the temp node
      adjacency_node_t *adj_node = temp->head; // set adj_node to the first element of the adjacency list
      while (adj_node != NULL)                 // while the adj_node has a next node
      {
        adjacency_node_t *temp_adj = adj_node;                 // set temp_adj to the adj_node
        adj_node = adj_node->next;                             // set adj_node to the next node
        printf("%s %s\n", temp->word, temp_adj->vertex->word); // print the word and the word in the adjacency list")
      }
    }
  }
  return 0; */

  // ask what to do
  for (;;)
  {
    fprintf(stderr, "Your wish is my command:\n");
    fprintf(stderr, "  1 WORD       (list the connected component WORD belongs to)\n");
    fprintf(stderr, "  2 FROM TO    (list the shortest path from FROM to TO)\n");
    fprintf(stderr, "  3            (hash info)\n");
    fprintf(stderr, "  4            (graph info)\n");
    fprintf(stderr, "  5            (print words)\n");
    fprintf(stderr, "  6            (terminate)\n");
    fprintf(stderr, "> ");
    if (scanf("%99s", word) != 1)
      break;
    command = atoi(word);
    if (command == 1)
    {
      if (scanf("%99s", word) != 1)
        break;
      list_connected_component(hash_table, word, 1);
    }
    else if (command == 2)
    {
      if (scanf("%99s", from) != 1)
        break;
      if (scanf("%99s", to) != 1)
        break;
      path_finder(hash_table, from, to);
    }
    else if (command == 3)
    {
      hash_table_info(hash_table);
    }
    else if (command == 4)
    {
      graph_info(hash_table);
    }
    else if (command == 5)
    {
      int numPalavras = 0;
      for (i = 0; i < hash_table->hash_table_size; i++)
      {
        for (node = hash_table->heads[i]; node != NULL; node = node->next)
        {
          printf("indice = %u -> %s\n", hash_table->heads[i], node->word);
          numPalavras++;
        }
      }

      printf("Número de palavras = %d\n", numPalavras);
    }
    else if (command == 6)
      break;
    else
    {
      fprintf(stderr, "Invalid command\n");
    }
  }
  // clean up
  hash_table_free(hash_table);
  return 0;
}
