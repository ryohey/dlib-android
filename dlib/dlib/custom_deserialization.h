
#include "image_processing/shape_predictor.h"
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

namespace dlib
{
    void deserializeMinified(shape_predictor &sp, std::string filepath) {
        // load the shape predictor from the packed file:
        auto file = std::fopen(filepath.c_str(), "rb");

        cout << "reading initial shape" << endl;

        // read in the initial shape:
        sp.initial_shape.set_size(136, 1);
        float floatBuf[136];
        fread(floatBuf, 4, 136, file);
        for(int r = 0; r < 136; r++)
        {
            sp.initial_shape(r, 0) = floatBuf[r];
        }

        cout << "reading forests" << endl;

        // read in the forests:
        sp.forests.resize(15);
        for(int idx = 0; idx < 15; idx++)
        {
            sp.forests[idx].resize(500);
            int tree;

            for(tree = 0; tree < 500; tree++)
            {
                // cout << "reading splits" << endl;

                // read in the splits:
                sp.forests[idx][tree].splits.resize(15);
                for(int split = 0; split < 15; split++)
                {
                    unsigned int idx1, idx2;
                    float thresh;
                    fread(&idx1, 4, 1, file);
                    fread(&idx2, 4, 1, file);
                    fread(&thresh, 4, 1, file);

                    sp.forests[idx][tree].splits[split].idx1 = (unsigned long)idx1;
                    sp.forests[idx][tree].splits[split].idx2 = (unsigned long)idx2;
                    sp.forests[idx][tree].splits[split].thresh = thresh;
                }

                // cout << "reading leaf_values" << endl;

                sp.forests[idx][tree].leaf_values.resize(16);
                int leaf;
                for(leaf = 0; leaf < 16; leaf++)
                {
                    sp.forests[idx][tree].leaf_values[leaf].set_size(136, 1);

                    float leafmin, step;
                    fread(&leafmin, 4, 1, file);
                    fread(&step, 4, 1, file);

                    unsigned char quantized[136];
                    fread(quantized, 136, 1, file);
                    for(int r = 0; r < 136; r++)
                    {
                        int q = (int)quantized[r] & 255;
                        float value = leafmin + ((float)q * step);
                        sp.forests[idx][tree].leaf_values[leaf](r, 0) = value;
                    }
                }
            }
        }

        cout << "reading anchor_idx" << endl;

        // read in anchor_idx:
        sp.anchor_idx.resize(15);
        for(int idx = 0; idx < 15; idx++)
        {
            sp.anchor_idx[idx].resize(500);
            for(int aidx = 0; aidx < 500; aidx++)
            {
                unsigned int val;
                fread(&val, 4, 1, file);
                sp.anchor_idx[idx][aidx] = (unsigned long)val;
            }
        }

        cout << "reading deltas" << endl;

        // read in the deltas:
        sp.deltas.resize(15);
        for(int idx = 0; idx < 15; idx++)
        {
            sp.deltas[idx].resize(500);
            for(int didx = 0; didx < 500; didx++)
            {
                float x, y;
                fread(&x, 4, 1, file);
                fread(&y, 4, 1, file);
                sp.deltas[idx][didx].x() = x;
                sp.deltas[idx][didx].y() = y;
            }
        }

        cout << "closing file" << endl;

        fclose(file);
    }
}
