#pragma once

#include <iostream>
#include <cmath>
#include <list>
#include <vector>
#include <memory>
#include "triangle.hpp"


namespace Tree {
    const double MINIMUM_SIZE = 0.1;

    class OctreeNode;
    using NodePtr = std::unique_ptr<OctreeNode>;

    class OctreeNode final 
    {
    private:
        //Data--------------------------------------------------------------------------
        GeomObj::Vector rightBorder_, leftBorder_;
        std::vector<NodePtr> child_;
        std::list<GeomObj::Triangle> listOfTriangles_;
        //------------------------------------------------------------------------------

        //Private methods---------------------------------------------------------------
        int whatChapter(GeomObj::Vector& leftBorder, GeomObj::Vector& rightBorder, const GeomObj::Triangle& tr);
        void createNewNode(int chapter);
        void siftTree();
        void checkSubtree(GeomObj::Triangle& tr, std::vector<bool>& intersectTriangleFlagArray, int& SubtreeCounter);
        //------------------------------------------------------------------------------

    public:
        //Constructors------------------------------------------------------------------
        OctreeNode(GeomObj::Vector right = GeomObj::Vector{0,0,0}, GeomObj::Vector left = GeomObj::Vector{0,0,0}):
                rightBorder_{right},
                leftBorder_{left},
                child_{8}
                {}

        OctreeNode(const OctreeNode& rhs)            = delete;         // ban copy ctor
        OctreeNode(OctreeNode&& rhs)                 = delete;         // ban move ctor
        OctreeNode& operator=(OctreeNode&& rhs)      = delete;         // ban move assignment
        OctreeNode& operator=(const OctreeNode& rhs) = delete;         // ban assignment operator

        //Methods-----------------------------------------------------------------------
        GeomObj::Vector getRightBorder() const noexcept {return rightBorder_;}
        GeomObj::Vector getLeftBorder () const noexcept {return leftBorder_;}
        const std::list<GeomObj::Triangle>& trianglesList() const noexcept {return listOfTriangles_;}

        int IntersectionCounter(std::vector<bool>& intersectTriangleFlagArray);
        void fillTree(const std::vector<GeomObj::Triangle>& triangles);
        void dumpTree();

    };


    class Octree final 
    {
    private:
        NodePtr root;
    public:
        Octree() : root {std::make_unique<OctreeNode>()} {}

        Octree(const Octree& other) = delete;                  
        Octree(Octree&& other)      = delete;                  
        
        Octree& operator=(const Octree& other) = delete;       
        Octree& operator=(Octree&& other)      = delete;       

        void push(const std::vector<GeomObj::Triangle>& triangles) {
            root->fillTree(triangles);
        }
        
        int IntersectionCounter(std::vector<bool>& intersectTriangleFlagArray) {
            return root->IntersectionCounter(intersectTriangleFlagArray);
        }

        double getMaxCoor() const noexcept {
            return root->getRightBorder().getAbsMaxCoord();
        }
    };
};

