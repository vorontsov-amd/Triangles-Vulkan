#include "octree.hpp"
#include "utility.hpp"

namespace Tree {
    int OctreeNode::whatChapter(GeomObj::Vector& leftBorder, GeomObj::Vector& rightBorder, const GeomObj::Triangle &tr) {
        int chapter[3] {};

        for (int vertexNum = 0; vertexNum < 3; ++vertexNum) {
            GeomObj::Vector tmp = tr[vertexNum];

            for (int coordNum = 0; coordNum < 3; ++coordNum) {  
                double mid = ((leftBorder + rightBorder) / 2.0)[coordNum];

                if (tmp[coordNum] > mid) {
                    chapter[vertexNum] |= 1 << coordNum;
                }
                else if (isEqual(tmp[coordNum], mid)) {
                    return -1;
                }
            }

            if (vertexNum) {
                if(chapter[vertexNum] != chapter[vertexNum - 1]) {
                    return -1;
                }
            }
        }
        return chapter[0];
    }

//-----------------------------------------------------------------------------------------------------

    void OctreeNode::createNewNode(OctreeNode& curRoot, int chapter) {

        curRoot.child[chapter] = new OctreeNode {};
        curRoot.child[chapter]->parent_ = &curRoot;
        
        GeomObj::Vector right = curRoot.rightBorder_;
        GeomObj::Vector left  = curRoot.leftBorder_;
        GeomObj::Vector mid   = (right + left) / 2.0;

        for(int i = 0; i < 3; ++i) {
            if((chapter >> i) & 1) {
                curRoot.child[chapter]->leftBorder_[i]  = mid[i];
                curRoot.child[chapter]->rightBorder_[i] = right[i];
            }
            else {
                curRoot.child[chapter]->leftBorder_[i]  = left[i];
                curRoot.child[chapter]->rightBorder_[i] = mid[i];
            }
        }
    }

//-----------------------------------------------------------------------------------------------------

    void OctreeNode::siftTree(OctreeNode &curRoot) {
        if (curRoot.listOfTriangles.size() <= 2 || (curRoot.rightBorder_ - curRoot.leftBorder_).squareLength() < MINIMUM_SIZE) {
            return;
        }

        ListIt it = curRoot.listOfTriangles.begin();
        while (it != curRoot.listOfTriangles.end()) {
            int chapter = whatChapter (curRoot.leftBorder_, curRoot.rightBorder_, *it);

            if (chapter < 0) {
                ++it;
                continue;
            }

            if (!curRoot.child[chapter])
                createNewNode (curRoot, chapter);

            ListIt tmp = it;

            if (it != curRoot.listOfTriangles.end())
                tmp = next(it);
            
            curRoot.child[chapter]->listOfTriangles.splice(curRoot.child[chapter]->listOfTriangles.begin(), curRoot.listOfTriangles, it);

            it = tmp;
        }

        for (int i = 0; i < 8; ++i) {
            if(!curRoot.child[i]) {
                continue;
            }

            siftTree(*(curRoot.child[i]));
        }
    }

//-----------------------------------------------------------------------------------------------------

    void OctreeNode::fillTree(const std::vector<GeomObj::Triangle>& triangles) {			
        double 	maxInTriangle   = 0, 
                maxInTree       = 0;

        GeomObj::Triangle tmp;
        for (size_t i = 0; i < triangles.size (); ++i) {

            tmp = triangles[i];
            tmp.number = i;

            listOfTriangles.push_front (tmp);

            maxInTriangle = tmp.getAbsMaxCoord ();
            if (maxInTriangle > maxInTree)
                maxInTree = maxInTriangle;
        }

        rightBorder_    = {maxInTree, maxInTree, maxInTree};
        leftBorder_     = GeomObj::Vector{0,0,0}-rightBorder_;

        siftTree (*this);
    }

//-----------------------------------------------------------------------------------------------------

    void OctreeNode::dumpTree (OctreeNode& curRoot) {
        std::cout << "left = " << curRoot.leftBorder_ << "; right = " << curRoot.rightBorder_ << std::endl;

        for(auto v : curRoot.listOfTriangles)
            std::cout << v << std::endl;

        std::cout << std::endl;

        for (int i = 0; i < 8; ++i) {
            if(!curRoot.child[i])
                continue;

            dumpTree (*(curRoot.child[i]));
        }
    }
}