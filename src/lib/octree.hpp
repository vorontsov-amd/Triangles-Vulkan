#pragma once

#include <iostream>
#include <cmath>
#include <list>
#include <vector>
#include "triangle.hpp"

namespace Tree {
    const double MINIMUM_SIZE = 0.1;

    class OctreeNode final 
    {
    private:
        //Data--------------------------------------------------------------------------
        GeomObj::Vector rightBorder_, leftBorder_;
        OctreeNode* parent_ {};
        //------------------------------------------------------------------------------

        //Private methods---------------------------------------------------------------
        int whatChapter(GeomObj::Vector &leftBorder, GeomObj::Vector &rightBorder, const GeomObj::Triangle &tr);
        void createNewNode(OctreeNode &curRoot, int chapter);
        void siftTree(OctreeNode &curRoot);

        void disactiveChild (OctreeNode* curChild) {
            for (int i = 0; i < 8; ++i) {
                if (curChild == child[i]) {
                    child[i] = nullptr;
                    break;
                }
            }   
        }
        //------------------------------------------------------------------------------

    public:
        //Public Data-------------------------------------------------------------------
        using ListIt = typename std::list<GeomObj::Triangle>::iterator;
        
        OctreeNode* child[8] {};
        std::list<GeomObj::Triangle>listOfTriangles;
        //------------------------------------------------------------------------------

        //Constructors------------------------------------------------------------------
        OctreeNode(GeomObj::Vector right = GeomObj::Vector{0,0,0}, GeomObj::Vector left = GeomObj::Vector{0,0,0}):
                rightBorder_{right},
                leftBorder_{left},
                child{}
                {}

        OctreeNode(const OctreeNode& rhs)            = delete;         // ban copy ctor
        OctreeNode(OctreeNode&& rhs)                 = delete;         // ban move ctor
        OctreeNode& operator=(OctreeNode&& rhs)      = delete;         // ban move assignment
        OctreeNode& operator=(const OctreeNode& rhs) = delete;         // ban assignment operator
        ~OctreeNode()                                = default; 
        //------------------------------------------------------------------------------

        //Methods-----------------------------------------------------------------------
        void deleteSubtree () {              
            OctreeNode* mainRoot = this;
            OctreeNode* curNode  = this;
            OctreeNode* toDelete = nullptr;

            while (curNode) {
                bool checker = false;
                for (int i = 0; i < 8; ++i) {
                    if (curNode->child[i]) {
                        
                        checker = true;
                    
                        curNode = curNode->child[i];
                        break;
                    }
                }   

                if (checker) {
                    continue;             
                }
                else if (curNode->parent_ && curNode != mainRoot) {                 
                    toDelete = curNode;
                    curNode  = curNode->parent_;
                    curNode->disactiveChild (toDelete);
                    delete toDelete;

                } 
                else {
                    OctreeNode* parent = curNode->parent_;
                    if (parent) {
                        parent->disactiveChild (curNode);
                    }
                    delete curNode;
                    return; 
                }
            }
            
        }

        GeomObj::Vector getRightBorder () const noexcept {return rightBorder_;}
        GeomObj::Vector getLeftBorder  () const noexcept {return leftBorder_;}

        //-----------------------------------------------------------------------------------------------------

        void fillTree   (const std::vector<GeomObj::Triangle>& triangles);
        void dumpTree   (OctreeNode &curRoot);

    };

    class Octree final 
    {
    private:
        OctreeNode* root = nullptr;
    public:
        Octree() : root{new OctreeNode} {}
        ~Octree() {root->deleteSubtree ();}    

        Octree(const Octree& other) = delete;                  
        Octree(Octree&& other)      = delete;                  
        
        Octree& operator=(const Octree& other) = delete;       
        Octree& operator=(Octree&& other)      = delete;       

        void push(const std::vector<GeomObj::Triangle>& triangles) {
            root->fillTree(triangles);
        }
        
        OctreeNode* getRoot() const noexcept {return root;}

        double getMaxCoor() const noexcept {return (root->getRightBorder().getAbsMaxCoord());}
    };
}
