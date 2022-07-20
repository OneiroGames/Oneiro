//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Renderer/Vulkan/Buffer.hpp"
#include "Oneiro/Renderer/Vulkan/PhysicalDevice.hpp"

#include <stdexcept>

#include "Oneiro/Renderer/Renderer.hpp"
#include "Oneiro/Renderer/Vulkan/CommandBuffer.hpp"
#include "Oneiro/Renderer/Vulkan/LogicalDevice.hpp"

namespace oe::Renderer::Vulkan
{
    void Buffer::Create(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer,
                        VkDeviceMemory& bufferMemory)
    {
        const auto device = GetLogicalDevice()->Get();
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        VK_CHECK_RESULT(vkCreateBuffer(device, &bufferInfo, nullptr, &buffer), "Failed to create buffer!")

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);

        VK_CHECK_RESULT(vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory), "Failed to allocate buffer memory!")

        vkBindBufferMemory(device, buffer, bufferMemory, 0);
    }

    void Buffer::Copy(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
    {
        const auto commandBuffer = CommandBuffer::BeginSingleTimeCommands();

        VkBufferCopy copyRegion{};
        copyRegion.size = size;
        vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

        CommandBuffer::EndSingleTimeCommands(commandBuffer);
    }

    uint32_t Buffer::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
    {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(GetPhysDevice()->Get(), &memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
                return i;

        OE_THROW_ERROR("Renderer::Vulkan", "Failed to find suitable memory type!")
    }

    void Buffer::CopyToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
    {
        const auto commandBuffer = CommandBuffer::BeginSingleTimeCommands();

        const VkBufferImageCopy region{0, 0, 0, {VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1}, {0}, {width, height, 1}};

        vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

        CommandBuffer::EndSingleTimeCommands(commandBuffer);
    }
}
// namespace oe::Renderer::Vulkan
