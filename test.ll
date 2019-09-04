; ModuleID = 'swi2else'
source_filename = "swi2else"

declare i32 @prototype_of_function(i32, double)

declare i32 @proto2()

define i32 @test_f(double %c) {
entry:
  %r = alloca double
  %c2 = alloca i32
  %c1 = alloca double
  store double %c, double* %c1
  store i32 0, i32* %c2
  store double 0.000000e+00, double* %r
  store i32 4, i32* %c2
  ret i32 8
}
