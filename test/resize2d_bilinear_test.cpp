#include "test_utlis.h"
extern "C" {
#include "src/nn/resize2d_bilinear.h"
}

int main(int argc, char** argv) {
  if (argc == 3) {
    Tensor input, output;
    Device device = {DEVICE_CPU, 0};
    int64_t input_ndim, result_ndim, input_dtype, result_dtype, input_num,
        result_num;
    std::vector<int64_t> input_dims_vector, result_dims_vector;
    std::vector<float> input_data_vector, result_data_vector;
    read_data(argv[1], &input_ndim, &input_dims_vector, &input_dtype,
              &input_num, &input_data_vector);

    int64_t* input_dims;
    input_dims = (int64_t*)malloc(sizeof(int64_t) * input_ndim);
    memset(input_dims, 0, sizeof(float) * input_ndim);
    for (int i = 0; i < input_ndim; i++) {
      input_dims[i] = input_dims_vector[i];
    }

    aitisa_create(aitisa_int_to_dtype(input_dtype), device, input_dims,
                  input_ndim, nullptr, 0, &input);
    assign(input, input_data_vector);
    aitisa_resize2d_bilinear(input, 500, 500, &output);

    auto output_data = (float*)aitisa_tensor_data(output);
    read_data(argv[2], &result_ndim, &result_dims_vector, &result_dtype,
              &result_num, &result_data_vector);

    int64_t output_size = aitisa_tensor_size(output);
    for (int i = 0; i < output_size; ++i) {
      //      std::cout << output_data[i] << std::endl;
      if (abs(output_data[i] - result_data_vector[i]) > 1e-3) {
        std::cout << " mismatch" << std::endl;
        exit(1);
      }
    }
    free(input_dims);
    aitisa_destroy(&input);
    aitisa_destroy(&output);
  } else if (argc > 3) {
    std::cout << "Too many arguments supplied." << std::endl;
    exit(1);
  } else {
    std::cout << "Two argument expected. Usage: ./resize2d_bilinear_test "
                 "<input_path> <result_path> "
              << std::endl;
    exit(1);
  }
}