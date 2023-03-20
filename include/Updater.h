/*************************************************************************************
 * 
 * 
 * Copyright (c) 2021, Zhengjun Liu <zjliu@casm.ac.cn>
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 ************************************************************************************/


#pragma once

#include "hslLIB.h"
#include <stdio.h>
#include <string>
#include <memory>
#include <vector>
#include "hslDefinitions.h"
#include "FileIO.h"
#include "Point.h"
#include "Filter.h"
#include "Transform.h"

namespace hsl
{

class LIBHSL_API Updater : public FileIO
{
public:
	Updater();
	Updater(std::string filename);
  	virtual ~Updater();

	virtual bool open();
	virtual void close();

    /// Provides access to current point record.
    /// @exception nothrow
    Point & getPoint() const;

    /// Fetches next point record in file.
    /// @exception may throw std::exception
    bool readNextPoint(bool readWaveform = false);

    /// Fetches n-th point record from file.
    /// @exception may throw std::exception
    Point & readPointAt(size_t n, bool readWaveform = false);

    /// Reinitializes state of the reader.
    /// @exception may throw std::exception
    void reset();

    /// Move to the specified point to start 
    /// ReadNextPoint operations
    /// @exception may throw std::exception
    bool seek(size_t n);

    /// Get the current point record index
    size_t currentIndex();

    /// Provides index-based access to point records.
    /// The operator is implemented in terms of ReadPointAt method
    /// and is not const-qualified because it updates file stream position.
    /// @exception may throw std::exception
    Point & operator[](std::size_t n);

    /// update point to file at current position.
    bool writePoint(const Point& point, bool updateWaveform = false);
    bool writePoint(Point& point, bool updateWaveform = false);

    // update the field value by id for the current point record
    bool writeFieldValuesById(FieldId id, const VariantArray& values);

    // update the field value by field index for the current point record
    bool writeFieldValue(size_t index, Variant& value);

    /// Sets filters that are used to determine whether or not to 
    /// keep a point that was read from the file.  Filters have *no* 
    /// effect for reading data at specific locations in the file.  
    /// They only affect reading ReadNextPoint-style operations
    /// Filters are applied *before* transforms.
    void setFilters(std::vector<hsl::FilterPtr> const& filters);

    /// Gets the list of filters to be applied to points as they are read
    std::vector<hsl::FilterPtr> getFilters() const;

    /// Sets transforms to apply to points.  Points are transformed in 
    /// place *in the order* of the transform list.
    /// Filters are applied *before* transforms.  
    void setTransforms(std::vector<hsl::TransformPtr> const& transforms);

    /// Gets the list of transforms to be applied to points as they are read
    std::vector<hsl::TransformPtr> getTransforms() const;

protected:
    bool filterPoint(hsl::Point const& p);
    void transformPoint(hsl::Point& p);

    /// Fetches the waveform data of the current point record in file.
    /// @exception may throw std::exception
    bool readWaveformData();

private:
    bool writeRawValueToField(const Field& field, const Variant& value);

private:
    bool            _needHeaderCheck;
    uint32_t        _size;
    uint32_t        _current;

    PointPtr        _point;

    std::vector<hsl::FilterPtr>     _filters;
    std::vector<hsl::TransformPtr>  _transforms;
    std::vector<uint8_t>::size_type _recordSize;
};


}