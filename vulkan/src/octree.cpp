#include "octree.hpp"
#include "utility.hpp"
#include "intersect.hpp"

namespace Tree {
    int OctreeNode::whatChapter(GeomObj::Vector& leftBorder, GeomObj::Vector& rightBorder, const GeomObj::Triangle& tr) {
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

    void OctreeNode::createNewNode(int chapter) {

        child_[chapter] = std::make_unique<OctreeNode>();
        
        GeomObj::Vector right = rightBorder_;
        GeomObj::Vector left  = leftBorder_;
        GeomObj::Vector mid   = (right + left) / 2.0;

        for(int i = 0; i < 3; ++i) {
            if((chapter >> i) & 1) {
                child_[chapter]->leftBorder_[i]  = mid[i];
                child_[chapter]->rightBorder_[i] = right[i];
            }
            else {
                child_[chapter]->leftBorder_[i]  = left[i];
                child_[chapter]->rightBorder_[i] = mid[i];
            }
        }
    }

//-----------------------------------------------------------------------------------------------------

    void OctreeNode::siftTree() {

        if (listOfTriangles_.size() <= 2 || (rightBorder_ - leftBorder_).squareLength() < MINIMUM_SIZE) {
            return;
        }

        auto it = listOfTriangles_.begin();
        while (it != listOfTriangles_.end()) {

            int chapter = whatChapter(leftBorder_, rightBorder_, *it);
            if (chapter < 0) {
                ++it;
                continue;
            }

            if (!child_[chapter])
                createNewNode(chapter);

            auto tmp = it;
            if (it != listOfTriangles_.end())
                tmp = next(it);
            
            child_[chapter]->listOfTriangles_.splice(child_[chapter]->listOfTriangles_.begin(), listOfTriangles_, it);
            it = tmp;
        }

        for (int i = 0; i < 8; ++i) {
            if(!child_[i]) {
                continue;
            }

            child_[i]->siftTree();
        }
    }

//-----------------------------------------------------------------------------------------------------

    void OctreeNode::fillTree(const std::vector<GeomObj::Triangle>& triangles) {			
        double 	maxInTriangle   = 0, 
                maxInTree       = 0;

        GeomObj::Triangle tmp;
        for (size_t i = 0; i < triangles.size(); ++i) {

            tmp = triangles[i];
            tmp.number = i;

            listOfTriangles_.push_front(tmp);

            maxInTriangle = tmp.getAbsMaxCoord ();
            if (maxInTriangle > maxInTree)
                maxInTree = maxInTriangle;
        }

        rightBorder_    = {maxInTree, maxInTree, maxInTree};
        leftBorder_     = -rightBorder_;

        siftTree();
    }

//-----------------------------------------------------------------------------------------------------

    void OctreeNode::dumpTree() {
        std::cout << "left = " << leftBorder_ << "; right = " << rightBorder_ << std::endl;

        for(auto v : listOfTriangles_)
            std::cout << v << std::endl;

        std::cout << std::endl;

        for (int i = 0; i < 8; ++i) {
            if(!child_[i])
                continue;

            child_[i]->dumpTree();
        }
    }

//-----------------------------------------------------------------------------------------------------

    void OctreeNode::checkSubtree(GeomObj::Triangle& tr, std::vector<bool>& intersectTriangleFlagArray, int& SubtreeCounter) {

        for (int i = 0; i < 8; ++i) {
            if (child_[i] == nullptr)
                continue;
            
            auto triangles = child_[i]->trianglesList();

            for (auto It = triangles.begin(), end = triangles.end(); It != end; ++It) {
                bool addSubtreeCounter = GeomObj::IntersectTriangles(tr, *It);
                SubtreeCounter += addSubtreeCounter;
                
                if(addSubtreeCounter) {
                    intersectTriangleFlagArray[It->number] = true;
                    intersectTriangleFlagArray[tr.number] = true;
                }
            }

            child_[i]->checkSubtree(tr, intersectTriangleFlagArray, SubtreeCounter);
        }

    }


    //-----------------------------------------------------------------------------------------------------

    int OctreeNode::IntersectionCounter(std::vector<bool>& intersectTriangleFlagArray) {

        int counter = 0;

        auto tr = listOfTriangles_;

        for (auto ItSlow = tr.begin(), SlowEnd = tr.end(); ItSlow != SlowEnd; ++ItSlow) {

            auto curIt = ItSlow;
            ++curIt;
            for (auto ItFast = curIt, FastEnd = tr.end(); ItFast != FastEnd; ++ItFast) {
                bool addCounter = GeomObj::IntersectTriangles(*ItSlow, *ItFast);
                counter += addCounter;

                if(addCounter) {
                    intersectTriangleFlagArray[ItFast->number] = true;
                    intersectTriangleFlagArray[ItSlow->number] = true;
                }
            }

            checkSubtree(*ItSlow, intersectTriangleFlagArray, counter);
        }

        for (int i = 0; i < 8; ++i) {
            if (!(child_[i]))
                continue;

            counter += child_[i]->IntersectionCounter(intersectTriangleFlagArray);
        }
        return counter;

    }

}