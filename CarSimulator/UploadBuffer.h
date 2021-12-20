#pragma once

template<typename T>
class UploadBuffer
{
public:
    UploadBuffer(ID3D12Device* device, UINT elementCount, bool isConstantBuffer) : m_IsConstantBuffer(isConstantBuffer)
    {
        m_nElementByteSize = sizeof(T);

        // Constant buffer elements need to be multiples of 256 bytes.
        // This is because the hardware can only view constant data 
        // at m*256 byte offsets and of n*256 byte lengths. 
        // typedef struct D3D12_CONSTANT_BUFFER_VIEW_DESC {
        // UINT64 OffsetInBytes; // multiple of 256
        // UINT   SizeInBytes;   // multiple of 256
        // } D3D12_CONSTANT_BUFFER_VIEW_DESC;
        if (isConstantBuffer)
            m_nElementByteSize = CalcConstantBufferByteSize(sizeof(T));

        auto tmp = device->CreateCommittedResource(
            &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
            D3D12_HEAP_FLAG_NONE,
            &CD3DX12_RESOURCE_DESC::Buffer(m_nElementByteSize * elementCount),
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&m_pUploadBuffer));

        m_pUploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&m_pMappedData));

        // We do not need to unmap until we are done with the resource.  However, we must not write to
        // the resource while it is in use by the GPU (so we must use synchronization techniques).
    }

    UploadBuffer(const UploadBuffer& rhs) = delete;
    UploadBuffer& operator=(const UploadBuffer& rhs) = delete;
    ~UploadBuffer()
    {
        if (m_pUploadBuffer != nullptr)
            m_pUploadBuffer->Unmap(0, nullptr);

        m_pMappedData = nullptr;
    }

    ID3D12Resource* Resource()const
    {
        return m_pUploadBuffer.Get();
    }

    void CopyData(int elementIndex, const T& data)
    {
        memcpy(&m_pMappedData[elementIndex * m_nElementByteSize], &data, sizeof(T));
    }

    UINT CalcConstantBufferByteSize(UINT byteSize)
    {
        return (byteSize + 255) & ~255;
    }

    UINT GetElementSize()
    {
        return m_nElementByteSize;
    }

private:
    Microsoft::WRL::ComPtr<ID3D12Resource> m_pUploadBuffer;
    BYTE* m_pMappedData = nullptr;

    UINT m_nElementByteSize = 0;
    bool m_IsConstantBuffer = false;
};