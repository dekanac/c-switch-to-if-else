; ModuleID = 'swi2else'
source_filename = "swi2else"

define double @func() {
entry:
  %p = alloca double
  %r = alloca double
  %c = alloca double
  %i = alloca i32
  store i32 0, i32* %i
  store i32 1, i32* %i
  store double 0.000000e+00, double* %c
  store double 0.000000e+00, double* %r
  store double 0.000000e+00, double* %p
  store double 3.200000e+00, double* %c
  br label %loop1

loop1:                                            ; preds = %ifcont, %entry
  %i1 = load i32, i32* %i
  %letmp = icmp ule i32 %i1, 7
  %booltmp = uitofp i1 %letmp to double
  %whilecond = fcmp one double %booltmp, 0.000000e+00
  br i1 %whilecond, label %loop2, label %afterloop

loop2:                                            ; preds = %loop1
  %c2 = load double, double* %c
  %addtmp = fadd double %c2, 1.000000e+00
  store double %addtmp, double* %c
  %addtmp4 = add i32 %i1, 1
  store i32 %addtmp4, i32* %i
  %eqtmp = icmp eq i32 %addtmp4, 4
  %booltmp6 = uitofp i1 %eqtmp to double
  %ifcond = fcmp one double %booltmp6, 0.000000e+00
  br i1 %ifcond, label %then, label %else

afterloop:                                        ; preds = %loop1
  ret double 0.000000e+00

then:                                             ; preds = %loop2
  %subtmp = fsub double %addtmp, 1.000000e+00
  store double %subtmp, double* %c
  br label %ifcont

else:                                             ; preds = %loop2
  %addtmp9 = fadd double %addtmp, 1.000000e+00
  store double %addtmp9, double* %c
  br label %ifcont

ifcont:                                           ; preds = %else, %then
  br label %loop1
}
