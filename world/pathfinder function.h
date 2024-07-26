#ifndef PATHFINDER_H
#define PATHFINDER_H
#include <vector>
#include <functional>
#include <queue>
#include <iostream>
#include <limits>
#include <cmath>
#include <memory>
#include "qloggingcategory.h"

/*
 * A_star has 2 template parameters, T and U
 * T is the Node object which needs to have the fields as defined by the concept validNodeType
 *    3 floats: f, g and h to store the cost parameters of the A* algorithm
 *    a boolean visited which is set when you have already been to this node (~ closed list)
 *    a pointer to a Node: prev: the nodefrom which you got to this position
 *    Node should derive from Tile, so you can call getValue()
 * U is a position, passed a pointer, something which derives from Tile (or a Tile)
 * The comparator is used by the priority_queue to put the element with the lowest total cost in front
 * since Nodes is a 1D vector representing a 2D grid, we need know the width of this grid
 * heurWeight is the heuristic weight paramters defined by the A* algorithm
 *
 * The function returns a list of moves (0 to 7) using following encoding. The central point is your current position
 *
 *         7 0 1
 *         6   2
 *         5 4 3
 */


template <typename T>
using Comparator = std::function<bool(const T&, const T&)>;

template <typename T>
using pq = std::priority_queue<T, std::vector<T>, Comparator<T>>;

template<typename U>
concept validPosType = requires(U * u)
{
  {u->getXPos()} -> std::same_as<int>;
  {u->getYPos()} -> std::same_as<int>;
};

template<typename T>
concept validNodeType = validPosType<T> && requires(T t)
{
  {T::f} -> std::same_as<float&>;
  {T::g} -> std::same_as<float&>;
  {T::h} -> std::same_as<float&>;
  {T::visited} -> std::same_as<bool&>;
  {T::prev} -> std::same_as<T*&>;
  {t.getValue()} -> std::same_as<float>;
};

template <validNodeType T, validPosType U>
std::vector<int> A_star(std::vector<T> & Nodes, const U * start, const U * destination, Comparator<T>& comp, unsigned int width, float heurWeight)
{
  QLoggingCategory pathfinderCategory("pathfinder", QtDebugMsg);
  pq<T> openList(comp);
  std::vector<int> path;
  T currentPathNode = Nodes.at(start->getYPos()*width+start->getXPos());
  T * parent = &(Nodes.at(start->getYPos()*width+start->getXPos()));
  Nodes.at(start->getYPos()*width+start->getXPos()).visited = true; // avoid to come here again
  while (currentPathNode.getXPos() != destination->getXPos()
         || currentPathNode.getYPos() != destination->getYPos())
  {
    int currentX = currentPathNode.getXPos();
    int currentY = currentPathNode.getYPos();
    qCDebug(pathfinderCategory) << "Node to continue... " << currentY << ", " << currentX;
    for (int deltaY = -1; deltaY <= 1; deltaY++)
      for (int deltaX = -1; deltaX <= 1; deltaX++){
        qCDebug(pathfinderCategory)<< "Checking : " << deltaX << ", " << deltaY;
        if (!(deltaX == 0 && deltaY == 0)){
          qCDebug(pathfinderCategory) << "Checking neighbour : " << deltaX << ", " << deltaY;
          checkNeighbour(Nodes, width, currentX+deltaX, currentY+deltaY, destination, parent, openList, heurWeight);
        }
      }
    if (openList.empty())
    {
      return std::vector<int>();
    }
    else
    {
      currentPathNode = openList.top();
      qCDebug(pathfinderCategory) << "currentPathNode from openlist: " << currentPathNode.getXPos() << ", " << currentPathNode.getYPos() << ", f: " << currentPathNode.h;
      qCDebug(pathfinderCategory) << "Node on top of openlist: " << openList.top().h;
      qCDebug(pathfinderCategory) << "Size of openlist: " << openList.size();
      parent = &(Nodes.at(currentPathNode.getYPos()*width+currentPathNode.getXPos()));
      openList.pop();
    }
  }
  //return list of path to reach destination
  T * dest = parent;
  //std::cout << "Getting list of moves to take." <<std::endl;
  while (dest->prev != nullptr)
  {
    int deltaX = dest->prev->getXPos() - dest->getXPos();
    int deltaY = dest->prev->getYPos() - dest->getYPos();
    int newMove = deltaX+10*deltaY;
    qCDebug(pathfinderCategory) << "X:" << deltaX << ", Y:" << deltaY;
    switch (newMove)
    {
      case -10: path.push_back(4); break;
      case -11: path.push_back(5); break;
      case -1: path.push_back(6); break;
      case 9: path.push_back(7); break;
      case 10: path.push_back(0); break;
      case 11: path.push_back(1); break;
      case 1: path.push_back(2); break;
      case -9: path.push_back(3); break;
      default: std::cerr << "wrong value for deltaX" << deltaX;
    }
    dest = dest->prev;
  }
  std::reverse(path.begin(), path.end());
  return path;
}


template <validNodeType T, typename U>
void checkNeighbour(std::vector<T> & Nodes, int width, int nX, int nY,  const U *destination, T* parent, pq<T> &openList, float heurWeight)
{
  QLoggingCategory pathfinderCategory("pathfinder", QtDebugMsg);
  int height = Nodes.size() / width;
  unsigned int currentX = parent->getXPos();
  unsigned int currentY = parent->getYPos();
  float currentCost = parent->f;
  if (nX >= 0 && nX < width && nY >= 0 && nY < height && (nX != 0 || nY != 0))
  {
    if (Nodes.at(nY*width+nX).getValue()!=std::numeric_limits<float>::infinity() ) //top
    {
      int manhDist = abs(static_cast<int>(destination->getXPos())-nX) + abs(static_cast<int>(destination->getYPos())-nY);
      float f = (std::fabs(Nodes.at(currentY*width+currentX).getValue() -
                           Nodes.at(nY*width+nX).getValue())) + 1.0f;
      qCDebug(pathfinderCategory) << "Manhattan distance from " << Nodes.at(currentY*width+currentX).getValue() << "x"
                                  <<   Nodes.at(nY*width+nX).getValue() << ";" << currentX << "x" << currentY << ";"
                                  << destination->getXPos() << "," << destination->getYPos() <<" to " << nX << "," << nY << " is " << manhDist
                                  << ", f is " << f;
      T & nextNode = Nodes.at(nY*width+nX);
      if (nextNode.visited == false)
      {
        //never been here
        //get distance to target
        nextNode.f = currentCost + f;
        nextNode.h = currentCost + f + heurWeight * manhDist;
        nextNode.prev = parent;
        nextNode.visited = true;
        qCDebug(pathfinderCategory)<< "Pushing in openlist... " << nX << ", " << nY << " currentCost:" << nextNode.h;
        openList.push(nextNode);
      }
      else
      {
        //check if current path is better than previous
        qCDebug(pathfinderCategory) << "Check if this path is better, currentCost:" << currentCost << ", f:" << f << ", nextnode.f:" << nextNode.f;
        if (currentCost + f + heurWeight * manhDist < (nextNode.h - 0.1f))
        {
          qCDebug(pathfinderCategory) << "This path is better, reparenting.";
          nextNode.f = currentCost + f;
          nextNode.h = currentCost + f + heurWeight * manhDist;
          nextNode.prev = parent;
          openList.push(nextNode);
        }
      }
    }
  }
  else
    qCDebug(pathfinderCategory) << "Not processing: " << nX << ", " << nY;
}
#endif // PATHFINDER_H
