; ModuleID = 'swi2else'
source_filename = "swi2else"

define double @func() {
entry:
  %c = alloca double
  %i = alloca i32
  store i32 0, i32* %i
  store i32 1, i32* %i
  store double 0.000000e+00, double* %c
  store double 3.200000e+00, double* %c
  br label %loop1

loop1:                                            ; preds = %loop2, %entry
  %i1 = load i32, i32* %i
  %letmp = icmp ule i32 %i1, 7
  %booltmp = uitofp i1 %letmp to double
  %whilecond = fcmp one double %booltmp, 0.000000e+00
  %c2 = load double, double* %c
  br i1 %whilecond, label %loop2, label %afterloop

loop2:                                            ; preds = %loop1
  %addtmp = fadd double %c2, 1.000000e+00
  store double %addtmp, double* %c
  %addtmp4 = add i32 %i1, 1
  store i32 %addtmp4, i32* %i
  br label %loop1

afterloop:                                        ; preds = %loop1
  ret double %c2
}
