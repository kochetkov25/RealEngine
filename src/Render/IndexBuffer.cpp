#include "IndexBuffer.h"

namespace Render {

IndexBuffer::~IndexBuffer() { glDeleteBuffers(1, &_id); }

void IndexBuffer::bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id); }

void IndexBuffer::unbind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

} // namespace Render