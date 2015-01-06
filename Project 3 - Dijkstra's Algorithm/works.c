#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//Data Structure for List of Vertices
typedef struct ListNodeVertix
{
  int vertix;
  int visited;
  double distance;
  int x;
  int y;
  struct ListNodeVertix * prev;
  struct ListNodeVertix * next;
  struct ListNodeEdge * firstEdge;
} VertixNodeList;

//Data Structure for List of Edges, pointed at by each vertix
typedef struct ListNodeEdge
{
  int edge;
  struct ListNodeEdge * next;
} EdgeNodeList;

//Creating a Vertix Node to be added to the Vertix List
VertixNodeList * VertixList_createNode(int vertix, int x, int y)
{
  VertixNodeList * ptr_VertixList = NULL; 
  ptr_VertixList = malloc(sizeof(VertixNodeList));
  if (ptr_VertixList == NULL)
    {
      printf("MEMORY NOT ALLOCATED");
    }
  ptr_VertixList->vertix = vertix;
  ptr_VertixList->next = NULL;
  ptr_VertixList->firstEdge = NULL;
  ptr_VertixList->visited = 0;
  ptr_VertixList->distance = 10001;
  ptr_VertixList->x = x;
  ptr_VertixList->y = y;
  return ptr_VertixList;
}

//Creating an Edge Node to be added to the Edge List
EdgeNodeList * EdgeList_createNode(int edge)
{
  EdgeNodeList * ptr_EdgeList = NULL;
  ptr_EdgeList = malloc(sizeof(EdgeNodeList));
  if (ptr_EdgeList == NULL)
    {
      printf("MEMORY NOT ALLOCATED");
    }
  ptr_EdgeList->edge = edge;
  ptr_EdgeList->next = NULL;
  return ptr_EdgeList;
}

//Helper function to help print entire list, prints the edges
void printEdgeList(EdgeNodeList * head)
{
  if(head != NULL)
    {
      printf(" %d", head->edge);
      head = head->next;
      printEdgeList(head);
    }
}

//Function to print the entire list
void printList(VertixNodeList * head)
{
  if(head != NULL)
    {
      printf("%d | X:%d | Y:%d |: ", head->vertix, head->x, head->y);
      printEdgeList(head->firstEdge);
      printf("\n");
      head = head->next;
      printList(head);
    }
}

//Function to search through list given the head and a value (vertix), returns the poitner to the node
VertixNodeList * searchVertixList(VertixNodeList * head, int vertix)
{
  if(head->vertix == vertix)
    {
      return head;
    }
  else if(head->next != NULL)
    {
      head = head->next;
      head = searchVertixList(head,vertix);
    }
  return head;

}

//Helper function to compare edge's before adding to ensure edges are added in ascending order
void compareAndAdd(VertixNodeList * vertixSearched, EdgeNodeList * edgeAdd, int edgeValue, EdgeNodeList * edgeTemp, EdgeNodeList * edgeTempPrev)
{
  if(edgeValue == edgeTemp->edge)
    {
      edgeAdd->next = edgeTemp->next;
      edgeTemp->next = edgeAdd;
      
    }
  else if(edgeValue < edgeTemp->edge)
    {
      edgeTempPrev->next = edgeAdd;
      edgeAdd->next = edgeTemp;
    }
  else if(edgeTemp->next == NULL)
    {
      edgeTemp->next = edgeAdd;
    }

  else if(edgeTemp->next != NULL)
    {
      compareAndAdd(vertixSearched, edgeAdd, edgeValue, edgeTemp->next, edgeTemp);	  
    }
  
}

//Function to add edges, edges are added in ascending order
void addEdge(VertixNodeList * vertixSearched, EdgeNodeList * edgeTemp, EdgeNodeList * edgeAdd, int edgeValue)
{
  edgeAdd = EdgeList_createNode(edgeValue);
  edgeTemp = vertixSearched->firstEdge;
  EdgeNodeList * edgeTempPrev = NULL;
  
  if(edgeTemp != NULL)
    {
      if(edgeValue < edgeTemp->edge)
	{
	  vertixSearched->firstEdge = edgeAdd;
	  edgeAdd->next = edgeTemp;
	}
      else if(edgeTemp->next == NULL)
	{
	  edgeTemp->next = edgeAdd;
	}
      else
	{
	  edgeTempPrev = edgeTemp;
	  edgeTemp = edgeTemp -> next;
	  compareAndAdd(vertixSearched, edgeAdd, edgeValue, edgeTemp, edgeTempPrev);      
	}
    }
  else
    {
      vertixSearched->firstEdge = edgeAdd;
    }
}

void destroyEdges(EdgeNodeList * edgeNode)
{
  if(edgeNode->next != NULL)
    {
      destroyEdges(edgeNode->next);
    }
  
  free(edgeNode);
}

void destroyAll(VertixNodeList * vertixHead)
{
  if(vertixHead->next != NULL)
    {
      destroyAll(vertixHead->next);
    }
  if(vertixHead->firstEdge != NULL)
    {
      destroyEdges(vertixHead->firstEdge);
    }
  
  free(vertixHead);
}

//Calculating distance between two vertix nodes
double calculateDistance(VertixNodeList * source, VertixNodeList * destination)
{
  double distance;
  distance = pow((pow((destination->x - source->x),2))+(pow((destination->y - source->y),2)),0.5);
  return distance;
}

int main(int argc, char ** argv)
{
  //File name for map
  const char * filename = argv[1];
  //File name for query
  const char * query_filename = argv[2];
 
  //Setting FILE pointer for map
  FILE * fptr = NULL;
  //Opening map file
  fptr = fopen(filename, "r");

  //Setting FILE pointer for query
  FILE * query_fptr = NULL;

  //Initializing Variables
  int numVertices = 0; 
  int numEdges = 0;
  int countVertices = 0;
  int countEdges = 0;

  //Scans in and sets the number of vertices and number of edges
  fscanf(fptr, "%d %d\n", &numVertices, &numEdges);

  //This chunk deals with creating a linked list of vertices from the file. Uses the |Vertix Origin End| and pulls only Vertix
  int fileVertix, fileVertix_X, fileVertix_Y;

  fscanf(fptr, "%d %d %d\n", &fileVertix, &fileVertix_X, &fileVertix_Y);
  VertixNodeList * vertixHead = VertixList_createNode(fileVertix, fileVertix_X, fileVertix_Y);
  
  //temp nodes to be used to construct list
  VertixNodeList * vertixOne = vertixHead;
  VertixNodeList * vertixTwo = NULL;
  
  //Creating the vertix list
  for(countVertices = 0; countVertices < numVertices-1; countVertices++)
    {
      fscanf(fptr, "%d %d %d\n", &fileVertix, &fileVertix_X, &fileVertix_Y);
      vertixTwo = VertixList_createNode(fileVertix, fileVertix_X, fileVertix_Y);
      vertixOne->next = vertixTwo;
      vertixOne = vertixTwo;
    }

  //adding edges and edge lists
  int fileEdge = 0;
  int currVertix = -1;
  EdgeNodeList * edgeAdd = NULL;
  EdgeNodeList * edgeTemp = NULL;
  VertixNodeList * vertixSearched = NULL;
  VertixNodeList * vertixPairSearched = NULL;

  //Getting the first pair values. 
  fscanf(fptr, "%d %d\n", &fileVertix, &fileEdge);

  //Setting the current value of vertix to prevent searching every time. 
  currVertix = fileVertix;

  //Initial search to locate first pair of vertices
  vertixSearched = searchVertixList(vertixHead,fileVertix);
  vertixPairSearched = searchVertixList(vertixHead,fileEdge);

  //Creating the first pair of vertices
  addEdge(vertixSearched, edgeTemp, edgeAdd, fileEdge);
  addEdge(vertixPairSearched, edgeTemp, edgeAdd, fileVertix);

  for(countEdges = 0; countEdges < numEdges-1; countEdges++)
    {
      fscanf(fptr, "%d %d\n", &fileVertix, &fileEdge);
      
      //Checking if the new input is for the current vertix as before or a different one
      if(currVertix != fileVertix)
	{
	  //Finding the new vertix
	  vertixSearched = searchVertixList(vertixHead,fileVertix);
	  //Setting current vertix as the new vertix
	  currVertix = fileVertix; 
	}

      //Searching for the new vertix's pair vertix
      vertixPairSearched = searchVertixList(vertixHead,fileEdge);

      //Creating the pair
      addEdge(vertixSearched, edgeTemp, edgeAdd, fileEdge);
      addEdge(vertixPairSearched, edgeTemp, edgeAdd, fileVertix);
    }

  //printing list
  printList(vertixHead);

  //closing file pointer to map file, this is important
  fclose(fptr);

  //Opening query file
  query_fptr = fopen(query_filename, "r");

  //closing file pointer to map file, this is important
  fclose(query_fptr); 

  //Releasing all allocated memory
  destroyAll(vertixHead);
  return 0;
}
