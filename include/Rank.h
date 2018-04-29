#ifndef RANK_H
#define RANK_H

#define MAX_DOUBLE_VALUE 1E+38
#define TOLERANCE 1e-10

/**
 * Swaps two rows of the matrix.
 * @param matrix Matrix.
 * @param width Number of columns in the matrix.
 * @param height Number of rows in the matrix.
 * @param r1 First row.
 * @param r2 Second row.
 */
void swapRows(double* matrix, int width, int height, int r1, int r2) {
   float temp;
   int index1, index2;
   for (int c = 0; c < width; c++) {
      index1 = r1 * width + c;
      index2 = r2 * width + c;
      temp = matrix[index1];
      matrix[index1] = matrix[index2];
      matrix[index2] = temp;
   }
}

/**
 * Swaps two columns of the matrix.
 * @param matrix Matrix.
 * @param width Number of columns in the matrix.
 * @param height Number of rows in the matrix.
 * @param c1 First column.
 * @param c2 Second column.
 */
void swapColumns(double* matrix, int width, int height, int c1, int c2) {
   float temp;
   int index1, index2;
   for (int r = 0; r < height; r++) {
      index1 = r * width + c1;
      index2 = r * width + c2;
      temp = matrix[index1];
      matrix[index1] = matrix[index2];
      matrix[index2] = temp;
   }
}

/**
 * Computes absolute value.
 * @param f Input number.
 * @return Absolute value.
 */
double absoluteValue(double f) {
   return f >= 0 ? f : -f;
}

/**
 * Computes rank of the given matrix.
 * @param mx Matrix.
 * @param width Number of columns in the matrix.
 * @param height Number of rows in the matrix.
 * @param colId Column ids
 * @return Rank of the matrix.
 */
int computeRank(double* mx, int width, int height, int* colId, double* augmentation) {
   int prows = height;
   int pcols = width;
   int pivs = (prows < pcols) ? prows : pcols;
   int itemp;
   double dtemp;
   //find pivot row/column
   int prow = -1;
   int pcol = -1;
   float pval = -MAX_DOUBLE_VALUE;
   for (int row = 0; row < prows; row++) {
      for (int col = 0; col < pcols; col++) {
         float val = absoluteValue(mx[row * width + col]);
         if (val > pval) {
            pval = val;
            prow = row;
            pcol = col;
         }
      }
   }
   int pivot;
   //precondition (each iteration): prow/pcol/pval are set
   for (pivot = 0; pivot < pivs; pivot++) {
      if (pval <= TOLERANCE) break;
      //swap rows / columns
      if (prow != pivot) {
         swapRows(mx, width, height, prow, pivot);
         if (augmentation != NULL) {
            dtemp = augmentation[prow];
            augmentation[prow] = augmentation[pivot];
            augmentation[pivot] = dtemp;
         }
      }
      if (pcol != pivot) {
         swapColumns(mx, width, height, pcol, pivot);
         if (colId != NULL) {
            itemp = colId[pcol];
            colId[pcol] = colId[pivot];
            colId[pivot] = itemp;
         }
      }

      //divide pivot row
      pval = mx[pivot * width + pivot];
      for (int col = pivot + 1; col < width; col++) {
         mx[pivot * width + col] /= pval;
      }
      if (augmentation != NULL) {
         augmentation[pivot] /= pval;
      }
      mx[pivot * width + pivot] = 1;

      //subtract pivot row from other rows
      //find next pivot at the same time
      pval = -MAX_DOUBLE_VALUE;
      for (int row = pivot + 1; row < height; row++) {
         float rpiv = mx[row * width + pivot];
         mx[row * width + pivot] = 0;
         for (int col = pivot + 1; col < width; col++) {
            float val = mx[row * width + col];
            float sub = mx[pivot * width + col];
            val -= sub * rpiv;
            mx[row * width + col] = val;
            // is this our new pivot?
            if (val < 0.0) {
               val = -val;
            }
            if (val > pval) {
               pval = val;
               prow = row;
               pcol = col;
            }
         }
         if (augmentation != NULL) {
            augmentation[row] -= rpiv * augmentation[pivot];
         }
      }
   }
   return pivot;
}


#endif
