/*
 *
 *   Copyright (C) 2013 BUAA iTR Research Center. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * tracker.h
 *  Created on: 2013-9-29
 *      Author: ghdawn
 */

#ifndef LKTRACKER_H_
#define LKTRACKER_H_

#include "itrbase.h"
#include "../itrvision.h"
using itr_math::Point2D;
namespace itr_vision
{

    class LKTracker
    {
        public:
            enum TrackResult
            {
                Tracked, OOB, SmallDet
            };
            LKTracker(const ImageGray& img1, const ImageGray& img2);
            virtual ~LKTracker();
            TrackResult Compute(Point2D& U, Point2D& V, int L, bool Forward);
            int windowWidth;
        private:
            void _ComputeDt(Point2D& U, Point2D& V, int L,int hw,S32* dt);
            void GeneratePyramidal(const ImageGray& img, ImageGray py[], S32 length);
            ImageGray I[3], J[3];
            S32 width[3],height[3];
    };

} // namespace itr_vision
#endif // TRACKER_H_
