#ifndef RENDER_COMMAND_H
#define RENDER_COMMAND_H

#include "Renderer.h"

namespace RKTEngine
{
	class RenderCommand
	{
		public:
			inline static void clearColor(Color color)
			{
				spRendererAPI->clearColor(color);
			}

			inline static void clearBuffer(int bufferClearFlags)
			{
				spRendererAPI->clearBuffer(bufferClearFlags);
			}

			inline static void setCullOrder(bool isClockwise)
			{
				spRendererAPI->setCullOrder(isClockwise);
			}

			inline static void setCullFace(int faceCullSetting)
			{
				spRendererAPI->setCullFace(faceCullSetting);
			}

			inline static void setDepthBuffer(int depthBufferSetting)
			{
				spRendererAPI->setDepthBuffer(depthBufferSetting);
			}

			inline static void setStencilBuffer(int bufferComparison, int refValue, int mask)
			{
				spRendererAPI->setStencilBuffer(bufferComparison, refValue, mask);
			}

			inline static void setStencilMask(int mask)
			{
				spRendererAPI->setStencilMask(mask);
			}

			inline static void setActiveTexture(int channel, int offset = 0)
			{
				spRendererAPI->setActiveTexture(channel, offset);
			}

			inline static void drawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
			{
				spRendererAPI->drawIndexed(vertexArray);
			}

			inline static void drawTriangles(const std::shared_ptr<VertexArray>& vertexArray)
			{
				spRendererAPI->drawTriangles(vertexArray);
			}

			inline static void drawInstancedTriangles(const std::shared_ptr<VertexArray>& vertexArray, int instanceCount)
			{
				spRendererAPI->drawInstancedTriangles(vertexArray, instanceCount);
			}
			inline static void drawInstanced(const std::shared_ptr<VertexArray>& vertexArray, int instanceCount)
			{
				spRendererAPI->drawInstanced(vertexArray, instanceCount);
			}

		private:
			static Renderer* spRendererAPI;
	};
}
#endif // !RENDER_COMMAND_H
