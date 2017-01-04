# Sdet vs Dbdet


## `sdet_edgel`

sdet: tangent commment doesn't mention it is in radians or not. TODO: comment
  - confirmed: both are radians. Include comment patch to LEMS.

dbdet: has const_interator:

## `sdet_sel_utils`

sdet, dbdet: identical

## `sdet_appearance`

sdet, dbdet:identical


## `sdet_edgemap`

dbdet: has AlmostEqual

## `sdet_curvelet`

sdet: no read (which is empty anyways), otherwise identical to dbdet

## `sdet_curve_model`
- huge
- identical, except for some potentially useful comments retained in dbdet

## `sdet_curvelet_map`
- identical

## `curve_fragment_map`
same

## `sdet_CFTG`
same

## storage


## `dbet_convert_edgemap.h`
copyed to sdetd


TODO: `use_approx_sines` apparently not in VXL/sdet (should be in its cmake)
