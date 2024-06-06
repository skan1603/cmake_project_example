#include "context.h"

ContextUPtr Context::Create() {
  auto context = ContextUPtr(new Context());
  if (!context->Init())
    return nullptr;
  return std::move(context);
}

bool Context::Init() {

  float vertices[] = {
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
    0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,

    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
    0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
    0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

    0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
    0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
    0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
    0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, 0.0f, 0.0f,
  };

  uint32_t indices[] = {
    0,  2,  1,  2,  0,  3,
    4,  5,  6,  6,  7,  4,
    8,  9, 10, 10, 11,  8,
    12, 14, 13, 14, 12, 15,
    16, 17, 18, 18, 19, 16,
    20, 22, 21, 22, 20, 23,
  };

  // 1

  m_vertexLayout = VertexLayout::Create();
	//glGenVertexArrays(1, &m_vertexArrayObject);
  //glBindVertexArray(m_vertexArrayObject);

  // 2
  m_vertexBuffer = Buffer::CreateWithData(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices, sizeof(float) * 120);
  /*
  glGenBuffers(1, &m_vertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, vertices, GL_STATIC_DRAW);
  */

  // 3                    인덱스,  인덱스의 데이터 겟수, 데이터 타입,  vertex 하나의 크기 (STRIDE), 한 vertex 내에서의 위치(OFFSET)
  m_vertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);                           
  m_vertexLayout->SetAttrib(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, sizeof(float) * 3);
  //glEnableVertexAttribArray(0);
  //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

  m_indexBuffer = Buffer::CreateWithData(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices, sizeof(uint32_t)* 36);
  /*
  glGenBuffers(1, &m_indexBuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * 6, indices, GL_STATIC_DRAW);
  */
  ShaderPtr vertShader = Shader::CreateFromFile("./shader/texture.vs", GL_VERTEX_SHADER);
  ShaderPtr fragShader = Shader::CreateFromFile("./shader/texture.fs", GL_FRAGMENT_SHADER);

  if (!vertShader || !fragShader)
    return false;
  SPDLOG_INFO("vertex shader id: {}", vertShader->Get());
  SPDLOG_INFO("fragment shader id: {}", fragShader->Get());
  
  m_program = Program::Create({fragShader, vertShader});
  if (!m_program)
    return false;
  SPDLOG_INFO("program id: {}", m_program->Get());

/*
	auto loc = glGetUniformLocation(m_program->Get(), "color");
  m_program->Use();
  glUniform4f(loc, 1.0f, 1.0f, 0.0f, 1.0f);
*/


  auto image = Image::Load("./image/container.jpg");
  m_texture = Texture::CreateFromImage(image.get());
/*
  auto image = Image::Create(512, 512);
  image->SetCheckImage(16, 16);
  m_texture = Texture::CreateFromImage(image.get());

  auto image = Image::Load("./image/container.jpg");
  if (!image) 
    return false;
  SPDLOG_INFO("image: {}x{}, {} channels",
    image->GetWidth(), image->GetHeight(), image->GetChannelCount());
*/


  auto image2 = Image::Load("./image/awesomeface.png");
  m_texture2 = Texture::CreateFromImage(image2.get());

  /*
  glGenTextures(1, &m_texture);
  glBindTexture(GL_TEXTURE_2D, m_texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
      image->GetWidth(), image->GetHeight(), 0,
      GL_RGB, GL_UNSIGNED_BYTE, image->GetData());
  */

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_texture->Get());
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, m_texture2->Get());

  m_program->Use();
  m_program->SetUniform("tex", 0);
  m_program->SetUniform("tex2", 1);
  //glUniform1i(glGetUniformLocation(m_program->Get(), "tex"), 0);
  //glUniform1i(glGetUniformLocation(m_program->Get(), "tex2"), 1);
/*
  // 위치 (1, 0, 0)의 점. 동차좌표계 사용
  glm::vec4 vec(1.0f,  0.0f, 0.0f, 1.0f);
  // 단위행렬 기준 (1, 1, 0)만큼 평행이동하는 행렬
  auto trans = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 0.0f));
  // 단위행렬 기준 z축으로 90도만큼 회전하는 행렬
  auto rot = glm::rotate(glm::mat4(1.0f),
    glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
  // 단위행렬 기준 모든 축에 대해 3배율 확대하는 행렬
  auto scale = glm::scale(glm::mat4(1.0f), glm::vec3(3.0f));
  // 확대 -> 회전 -> 평행이동 순으로 점에 선형 변환 적용
  vec = trans * rot * scale * vec;
  SPDLOG_INFO("transformed vec: [{}, {}, {}]", vec.x, vec.y, vec.z);
*/

  // 0.5배 축소후 z축으로 90도 회전하는 행렬
  // x축으로 -55도 회전
  auto model = glm::rotate(glm::mat4(1.0f),
    glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
  // 카메라는 원점으로부터 z축 방향으로 -3만큼 떨어짐
  auto view = glm::translate(glm::mat4(1.0f),
    glm::vec3(0.0f, 0.0f, -3.0f));
  // 종횡비 4:3, 세로화각 45도의 원근 투영
  auto projection = glm::perspective(glm::radians(45.0f),
    (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.01f, 10.0f);

  //auto projection = glm::ortho(0.0f, (float)WINDOW_WIDTH, 0.0f, (float)WINDOW_HEIGHT, 0.01f, 10.0f);
  auto transform = projection * view * model;

//  auto transform = glm::mat4(1.0f);

 m_program->SetUniform("transform", transform);
//  auto transformLoc = glGetUniformLocation(m_program->Get(), "transform");  
//  glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

  return true;
}

void Context::Render() {
/*
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

  static float time = 0.0f;
  float t = sinf(time) * 0.5f + 0.5f;
  auto loc = glGetUniformLocation(m_program->Get(), "color");
*/
/*
  m_program->Use();

  auto projection = glm::perspective(glm::radians(45.0f),
      (float)640 / (float)480, 0.01f, 10.0f);
  auto view = glm::translate(glm::mat4(1.0f),
      glm::vec3(0.0f, 0.0f, -3.0f));
  auto model = glm::rotate(glm::mat4(1.0f),
      glm::radians((float)glfwGetTime() * 120.0f),
      glm::vec3(1.0f, 0.5f, 0.0f));
  auto transform = projection * view * model;
  m_program->SetUniform("transform", transform);

  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
*/


std::vector<glm::vec3> cubePositions = {
        glm::vec3( 0.0f, 0.0f, 0.0f),
        glm::vec3( 2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f, 2.0f, -2.5f),
        glm::vec3( 1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f),
    };

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    
    auto projection = glm::perspective(glm::radians(45.0f),
        (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.01f, 20.0f);
    auto view = glm::translate(glm::mat4(1.0f),
        glm::vec3(0.0f, 0.0f, -3.0f));

    for (size_t i = 0; i < cubePositions.size(); i++){
        auto& pos = cubePositions[i];
        auto model = glm::translate(glm::mat4(1.0f), pos);
        model = glm::rotate(model,
            glm::radians((float)glfwGetTime() * 120.0f + 20.0f * (float)i),
            glm::vec3(1.0f, 0.5f, 0.0f));
        auto transform = projection * view * model;
        m_program->SetUniform("transform", transform);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

/*
  glUniform4f(loc, t*t, 2.0f*t*(1.0f-t), (1.0f-t)*(1.0f-t), 1.0f);
  
    
  // glUseProgram(m_program->Get());
  // glDrawArrays(GL_TRIANGLES, 0, 6);

  time += 0.0016f;
*/
}