#ifndef BUFFER_H
#define BUFFER_H

#include <RKTUtils/Trackable.h>
#include "RocketEngine/Defines.h"
#include "RocketEngine/core/Log.h"

//		Classes and data for packing vertex buffer data
//	========================================================
//

namespace RKTEngine
{
	//Generalized shader types
	enum class ShaderDataType
	{
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	//get the size (in bytes)  of a data type
	static uint32 ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:			return 4;
			case ShaderDataType::Float2:		return 4 * 2;
			case ShaderDataType::Float3:		return 4 * 3;
			case ShaderDataType::Float4:		return 4 * 4;
			case ShaderDataType::Mat3:			return 4 * 3 * 3;
			case ShaderDataType::Mat4:			return 4 * 4 * 4;
			case ShaderDataType::Int:			return 4;
			case ShaderDataType::Int2:			return 4 * 2;
			case ShaderDataType::Int3:			return 4 * 3;
			case ShaderDataType::Int4:			return 4 * 4;
			case ShaderDataType::Bool:			return 1;
		}

		RKT_ASSERT(false); //Unknown data type 
		return 0;
	}

	//Data representing a single element of a vertex buffer layout
	struct BufferElement
	{
		std::string name;		//Passed to vertex shader for proper variable storage
		ShaderDataType type;
		uint32 offset, size, instanceIterations;
		bool normalized;

		BufferElement() {}

		BufferElement(ShaderDataType dataType, const std::string& elementName, int instanceIterations = 0, bool isNormalized = false)
			: name(elementName), type(dataType), size(ShaderDataTypeSize(dataType)), offset(0), instanceIterations(instanceIterations), normalized(isNormalized)
		{
		}

		//Returns the number of descrete values in the data type
		uint32 getComponentCount() const
		{
			switch (type)
			{
				case ShaderDataType::Float:		return 1;
				case ShaderDataType::Float2:	return 2;
				case ShaderDataType::Float3:	return 3;
				case ShaderDataType::Float4:	return 4;
				case ShaderDataType::Mat3:		return 3 * 3;
				case ShaderDataType::Mat4:		return 4 * 4;
				case ShaderDataType::Int:		return 1;
				case ShaderDataType::Int2:		return 2;
				case ShaderDataType::Int3:		return 3;
				case ShaderDataType::Int4:		return 4;
				case ShaderDataType::Bool:		return 1;
			}

			RKT_ASSERT(false); //Unknown data type
			return 0;
		}
	};

	//Encapsulation of vertex buffer data & declarations to be sent to the shader
	class BufferLayout
	{
		public:
			BufferLayout() {}

			BufferLayout(const std::initializer_list<BufferElement>& elements)
				: mElements(elements)
			{
				calculateOffsetAndStride();
			};

			inline const std::vector<BufferElement>& GetElements() const { return mElements; }
			inline uint32 getStride() const { return mStride; }

			//allows for use in a range-based for loop
			std::vector<BufferElement>::iterator begin() { return mElements.begin(); }
			std::vector<BufferElement>::iterator end() { return mElements.end(); }
			std::vector<BufferElement>::const_iterator begin() const { return mElements.begin(); }
			std::vector<BufferElement>::const_iterator end() const { return mElements.end(); }

		private:
			std::vector<BufferElement> mElements;
			uint32 mStride = 0;

			void calculateOffsetAndStride()
			{
				uint32 offset = 0;
				mStride = 0;

				for (auto& element : mElements)
				{
					element.offset = offset;
					offset += element.size;
					mStride += element.size;
				}
			}
		};

		//		abstract classes
		//	====================================
		//

		class VertexBuffer : public RKTUtil::Trackable
		{
			public:
				enum class DataType { STATIC, DYNAMIC, STREAM };

				virtual ~VertexBuffer() {}

				virtual void bind() const OVERRIDE_REQUIRED;
				virtual void unbind() const OVERRIDE_REQUIRED;

				//Used to update some portion of an existing buffer (call glBufferSubData)
				virtual void updateBufferData(float* vertices, uint32 size, int offset) OVERRIDE_REQUIRED;

				virtual void setLayout(const BufferLayout& layout) OVERRIDE_REQUIRED;
				virtual const BufferLayout& getLayout() const OVERRIDE_REQUIRED;

				virtual void setRenderData(const void* data, uint32_t size) OVERRIDE_REQUIRED;
			
				static VertexBuffer* create(uint32 size, DataType usage = DataType::DYNAMIC);
				static VertexBuffer* create(const float* vertices, uint32 size, DataType usage = DataType::STATIC);
				static VertexBuffer* create(const void* data, uint32 size, DataType usage = DataType::STATIC);
			private:
		};

		class IndexBuffer //Synonomous w/ EBOs
		{
			public:
				virtual ~IndexBuffer() {}

				virtual void bind() const OVERRIDE_REQUIRED;
				virtual void unbind() const OVERRIDE_REQUIRED;

				virtual uint32 getCount() const OVERRIDE_REQUIRED;

				static IndexBuffer* create(uint32* indicies, uint32 count);
			private:
		};

		class FrameBuffer
		{
			public:
				virtual ~FrameBuffer() {}

				virtual void bind()	const OVERRIDE_REQUIRED;
				virtual void bindScreenTexture() const OVERRIDE_REQUIRED;
				virtual void unbind() const OVERRIDE_REQUIRED;
				virtual void blit() const OVERRIDE_REQUIRED;
				virtual uint32 getTexture() const OVERRIDE_REQUIRED;

				static FrameBuffer* create(int width, int height, int aaSamples = 1);
			private:
	};
}

#endif