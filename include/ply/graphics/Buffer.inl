namespace ply {

///////////////////////////////////////////////////////////
template <typename T>
void Buffer::update(const std::vector<T>& data, uint32_t offset) {
    this->update(data.data(), data.size() * sizeof(T), offset);
}

///////////////////////////////////////////////////////////
template <typename T>
BufferBuilder& BufferBuilder::data(const std::vector<T>& data) {
    this->data(data.data(), data.size() * sizeof(T));
    return *this;
}

} // namespace ply